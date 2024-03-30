#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // prompt user for text
    string text = get_string("Text: ");

    // print number of letters
    int letters = count_letters(text);
    printf("Letters: %i\n", letters);

    // print number of words
    int words = count_words(text);
    printf("Words: %i\n", words);

    // print number of sentences
    int sentences = count_sentences(text);
    printf("Sentences: %i\n", sentences);

    // calculate percentage of words
    float avgW = words / 100.0;
    // calculate average of letters
    float L = letters / avgW;
    // calculate average of sentences
    float S = sentences / avgW;
    // apply coleman-liau index formula
    int index = round(0.0588 * L - 0.296 * S - 15.8);

    // print grade according to index
    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}

int count_letters(string text)
{
    int letters = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isalpha(text[i]))
        {
            letters++;
        }
    }
    return letters;
}

int count_words(string text)
{
    int words = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        if (isspace(text[i]))
        {
            words++;
        }
    }
    return words;
}

int count_sentences(string text)
{
    int sentences = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentences++;
        }
    }
    return sentences;
}