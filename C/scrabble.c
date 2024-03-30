#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // compare scores and print winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // keep track of score
    int score = 0;

    // get length of string
    int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        // set ASCII values
        int letter = word[i];

        // count uppercase points
        if (isupper(letter))
        {
            int position_upper = (letter - 65);
            score += POINTS[position_upper];
        }
        // count lowercase points
        else if (islower(letter))
        {
            int position_lower = (letter - 97);
            score += POINTS[position_lower];
        }
    }
    return score;
}
