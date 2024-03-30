from cs50 import get_string

text = get_string("Text: ")

# length of text
length = len(text)

# count letters
letters = 0
for i in range(length):
    if text[i].isalpha():
        letters += 1

words = 1
for i in range(length):
    if text[i].isspace():
        words += 1

sentences = 0
for i in range(length):
    if text[i] in [".", "!",  "?"]:
        sentences += 1

# calculate percentage of words
avgW = words / 100.0
# calculate average of letters
L = letters / avgW
# calculate average of sentences
S = sentences / avgW
# apply coleman-liau index formula
index = round(0.0588 * L - 0.296 * S - 15.8)

# print grade according to index
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")
