#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include<stdlib.h>
#include <string.h>

// function prototype
char rotate(string plaintext, int key);

int main(int argc, string argv[])
{
    int key;

    // check if user input is single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        // check if input is composed only of digits
        for (int input = 0; input < strlen(argv[1]); input++)
        {
            if (isdigit(argv[1][input]))
            {
                // convert input to key
                key = atoi(argv[1]);
            }
            else
            {
                printf("Key must be number\n");
                return 1;
            }
        }
    }
    // prompt for plaintext
    string plaintext = get_string("plaintext:  ");
    // set space for ciphertext
    printf("ciphertext: ");
    // encrypt text
    rotate(plaintext, key);
    return 0;
}

char rotate(string plaintext, int key)
{
    char c;
    char cipher;
    char cipher_upper;
    char cipher_lower;
    char cipher_other;

    for (int i = 0; i < strlen(plaintext); i++)
    {
        // rotate uppercase letters
        if (isupper(plaintext[i]))
        {
            c = (plaintext[i] + key) - 65;
            cipher_upper = (c % 26) + 65;
            printf("%c", cipher_upper);
        }
        // rotate lowercase letters
        else if (islower(plaintext[i]))
        {
            c = (plaintext[i] + key) - 97;
            cipher_lower = (c % 26) + 97;
            printf("%c", cipher_lower);
        }
        // keep non-letters in same position
        else
        {
            cipher_other = plaintext[i];
            printf("%c", cipher_other);
        }
    }
    printf("\n");
    return 0;
}