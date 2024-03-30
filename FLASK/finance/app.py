import os
import re

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    id = session["user_id"]

    balance = db.execute("SELECT cash FROM users WHERE id = ?", id)
    account = balance[0]["cash"]
    cash = "{:.2f}".format(account)

    portfolio = db.execute("SELECT * FROM portfolio WHERE user_id = ? ORDER BY symbol", id)
    if portfolio:
        portfolio_value = 0
        for company in portfolio:
            data = lookup(company["symbol"])
            symbol = data["symbol"]
            company["price"] = data["price"]

            holding = db.execute("SELECT shares FROM portfolio WHERE user_id = ? AND symbol = ?", id, symbol)
            company["holding_value"] = round(holding[0]["shares"] * company["price"], 2)
            portfolio_value += round(company["holding_value"], 2)
            portfolio_string = "{:.2f}".format(portfolio_value)

        return render_template("index.html", portfolio=portfolio, balance=cash, portfolio_value=portfolio_string, holding_value=company["holding_value"])

    else:
        return render_template("index.html", balance=cash, portfolio_value="")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not symbol:
            return apology("MISSING INPUT: SYMBOL")

        data = lookup(symbol)
        if data:
            name = data["symbol"]
            if symbol != name:
                return apology("SYMBOL NOT FOUND")

            try:
                shares = int(request.form.get("shares"))
                if not shares or shares < 1:
                    return apology("MISSING INPUT: SHARES")
            except ValueError:
                return apology("INPUT MUST BE NUMBER")

            id = session["user_id"]
            price = round((data["price"]), 2)
            balance = db.execute("SELECT cash FROM users WHERE id = ?", id)
            cash = round((balance[0]["cash"]), 2)
            cost = round((price * shares), 2)

            if cash < cost:
                return apology("NOT ENOUGH CASH")
            else:
                balance = round((cash - cost), 2)
                db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, id)
                db.execute("INSERT INTO history (symbol, shares, cost, user_id, transaction_type) VALUES (?, ?, ?, ?, ?)",
                           symbol, shares, cost, id, "purchase")

                portfolio = db.execute("SELECT * FROM portfolio WHERE user_id = ? AND symbol = (?)", id, symbol)

                if portfolio:
                    stock = portfolio[0]["shares"]
                    if stock:
                        stock += shares
                        db.execute("UPDATE portfolio SET shares = ? WHERE symbol = (?) AND user_id = ?", stock, symbol, id)
                    else:
                        stock = shares
                        db.execute("INSERT INTO portfolio (symbol, shares, user_id) VALUES (?, ?, ?)", symbol, shares, id)

                else:
                    stock = shares
                    db.execute("INSERT INTO portfolio (symbol, shares, user_id) VALUES (?, ?, ?)", symbol, shares, id)

                return redirect("/")
        else:
            return apology("SYMBOL NOT FOUND")
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    id = session["user_id"]

    history = db.execute("SELECT * FROM history WHERE user_id = ? ORDER BY TIMESTAMP DESC", id)
    costs = db.execute("SELECT cost FROM history WHERE user_id = ?", id)

    formatted_costs = []
    history_and_costs = []
    for cost in costs:
        formatted_cost = "{:.2f}".format(cost["cost"])
        formatted_costs.append(formatted_cost)
    history_and_costs = [{'timestamp': h['TIMESTAMP'], 'symbol': h['symbol'], 'transaction_type': h['transaction_type'],
                          'shares': h['shares'], 'cost': c} for h, c in zip(history, formatted_costs)]

    return render_template("history.html", history=history, history_and_costs=history_and_costs)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    if request.method == "POST":
        symbol = request.form.get("symbol")
        if symbol:
            data = lookup(symbol)

            if data:
                price = "{:.2f}".format(data["price"])
                return render_template("quoted.html", symbol=data["symbol"], price=price)

            else:
                return apology("SYMBOL NOT FOUND")
        else:
            return apology("SUBMIT A SYMBOL")

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "POST":

        username = request.form.get("username")
        if not username:
            return apology("MISSING INPUT: USERNAME")

        users = db.execute("SELECT * FROM users")
        for user in users:
            if username == user["username"]:
                return apology("USERNAME TAKEN")

        password = request.form.get("password")
        if not password:
            return apology("MISSING INPUT: PASSWORD")

        flag = 0
        while True:
            if (len(password) < 4):
                flag = -1
                break
            elif not re.search("[a-z]", password):
                flag = -1
                break
            elif not re.search("[0-9]", password):
                flag = -1
                break
            else:
                flag = 0
                break

        if flag == - 1:
            return apology("PASSWORD MUST HAVE AT LEAST 4 CHARACTERS, AT LEAST 1 LETTER AND 1 NUMBER")

        confirmation = request.form.get("confirmation")
        if not confirmation:
            return apology("MISSING INPUT: CONFIRMATION")
        elif confirmation != password:
            return apology("CONFIRMATION DOESN'T MATCH PASSWORD")

        hash = generate_password_hash(password)

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)

        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    id = session["user_id"]

    if request.method == "POST":

        symbol = request.form.get("symbol")
        if symbol is not None:
            data = lookup(symbol)
            name = data["symbol"]
            if not data:
                return apology("SUBMIT AN OPTION")
            if symbol != name:
                return apology("SYMBOL NOT FOUND")
        else:
            return apology("SUBMIT AN OPTION")

        try:
            shares = int(request.form.get("shares"))
            if not shares:
                return apology("MISSING INPUT: SHARES")
            if shares < 1:
                return apology("AT LEAST 1 SHARE")
        except ValueError:
            return apology("INPUT MUST BE NUMBER")

        portfolio = db.execute("SELECT * FROM portfolio WHERE user_id = ? AND symbol = ?", id, name)

        if portfolio:
            stock = portfolio[0]["shares"]
            if shares > stock:
                return apology("NOT ENOUGH SHARES")
            else:
                balance = db.execute("SELECT cash FROM users WHERE id = ?", id)
                cash = round((balance[0]["cash"]), 2)

                price = round((data["price"]), 2)
                cost = round((price * shares), 2)

                balance = round((cash + cost), 2)
                stock -= shares

                db.execute("UPDATE users SET cash = ? WHERE id = ?", balance, id)
                db.execute("INSERT INTO history (symbol, shares, cost, user_id, transaction_type) VALUES (?, ?, ?, ?, ?)",
                           name, shares, cost, id, "sale")

                if stock == 0:
                    db.execute("DELETE FROM portfolio WHERE symbol = ? AND user_id = ?", name, id)
                else:
                    db.execute("UPDATE portfolio SET shares = ? WHERE symbol = (?) AND user_id = ?", stock, name, id)

                return redirect("/")

        else:
            return apology("MISSING PORTFOLIO")

    else:
        portfolio = db.execute("SELECT * FROM portfolio WHERE user_id = ?", id)
        return render_template("sell.html", portfolio=portfolio)


@app.route("/balance", methods=["GET", "POST"])
@login_required
def balance():
    """Add cash to account balance."""

    id = session["user_id"]

    if request.method == "POST":
        option = request.form.get("value")
        if not option:
            return apology("MISSING INPUT: CASH")
        else:
            try:
                value = float(option)
            except ValueError:
                return apology("INPUT MUST BE NUMBER")

            balance = db.execute("SELECT cash FROM users WHERE id = ?", id)
            cash = round((balance[0]["cash"]), 2)
            new_balance = round((value + cash), 2)
            db.execute("UPDATE users SET cash = ? WHERE id = ?", new_balance, id)
            return redirect("/")

    else:
        return render_template("cash.html")
