from cs50 import get_string

card = get_string("Number: ")

# check length of number
length = len(card)
if length < 13 or length > 16:
    print("INVALID")

# list of digits
digits = []
for digit in card:
    digits.append(digit)

# get alternate digits
mults = []
singles = []
while length > 0:
    single_digit = digits[length - 1]
    singles.append(single_digit)
    length -= 1
    if length != 0:
        mult_digit = digits[length - 1]
        mults.append(mult_digit)
        length -= 1

# convert lists to numbers
mult_nums = [int(mult_num) for mult_num in mults]
single_nums = [int(single_num) for single_num in singles]

# get sum of single digits
single_sum = sum(single_nums)

# multiply digits
products = []
for mult_num in mult_nums:
    products.append(mult_num * 2)

# turn list of products into single number
prod = int(''.join(map(str, products)))

# add digits of products and get sum
prod_s = str(prod)
prod_l = len(prod_s)
prod_d = []
while prod_l > 0:
    dig = prod_s[prod_l - 1]
    prod_d.append(dig)
    prod_l -= 1
prod_digs = [int(dig) for dig in prod_d]
prod_sum = sum(prod_digs)

# get final sum and validate last digit
final_sum = single_sum + prod_sum
check = final_sum % 10

if check != 0:
    print("INVALID")
else:
    l = len(card)
    if l == 15 and digits[0] == "3" and digits[1] == "4" or digits[1] == "7":
        print("AMEX")
    elif l == 16 and digits[0] == "5" and digits[1] in ["1", "2", "3", "4", "5"]:
        print("MASTERCARD")
    elif l == 13 or l == 16 and digits[0] == "4":
        print("VISA")
    else:
        print("INVALID")
