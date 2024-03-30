#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// set constants to map key
const string alphabet_upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const string alphabet_lower = "abcdefghijklmnopqrstuvwxyz";

// function to encipher letters
void encrypt(string key, int length, string plaintext);

int main(int argc, string argv[])
{
    // check for invalid usage
    if (argc != 2)
    {
        printf("Usage: ./substitution [key]\n");
        return 1;
    }

    string key = argv[1];
    int length = strlen(key);

    // validate key length
    if (length != 26)
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }

    // check if key contains only alphabetical characters
    for (int i = 0; i < length; i++)
    {
        if (!isalpha(key[i]))
        {
            printf("Key must contain only alphabetical characters\n");
            return 1;
        }
    }

    // check if key contains repeated characters
    char temp;
    for (int i = 0; i < length; i++)
    {
        temp = key[i];
        for (int j = i + 1; j < length; j++)
        {
            if (key[j] == temp)
            {
                printf("Key can't contain repeated characters\n");
                return 1;
            }
        }
    }

    //prompt user for input
    string plaintext = get_string("plaintext: ");

    printf("ciphertext: ");

    // encipher text
    encrypt(key, length, plaintext);

    printf("\n");

    return 0;
}

void encrypt(string key, int length, string plaintext)
{
    char cipher;
    int dif;

    // loop through plaintext
    for (int i = 0; i < strlen(plaintext); i++)
    {
        // preserve non-alphabetical characters
        if (!isalpha(plaintext[i]))
        {
            cipher = plaintext[i];
            printf("%c", cipher);
        }
        else
        {
            // encipher uppercase letters
            for (int j = 0; j < length; j++)
            {
                // match current letter with uppercase map
                if (plaintext[i] == alphabet_upper[j])
                {
                    for (int k = 0; k < length; k++)
                    {
                        // if mapped letter appears as uppercase in key
                        if (alphabet_upper[j] == key[k])
                        {
                            // calculate distance between mapped position and key position
                            dif = k - j;
                            // set encrypted character as letter in mapped position, preserving plaintext case
                            cipher = toupper(key[k - dif]);
                            printf("%c", cipher);
                            break;
                        }
                        // if mapped letter appears as lowercase in key
                        if (alphabet_lower[j] == key[k])
                        {
                            // calculate distance between mapped position and key position
                            dif = k - j;
                            // set encrypted character as letter in mapped position, preserving plaintext case
                            cipher = toupper(key[k - dif]);
                            printf("%c", cipher);
                            break;
                        }
                    }
                }
                // encipher lowercase letters
                else if (plaintext[i] == alphabet_lower[j])
                {
                    // match current letter with lowercase map
                    for (int k = 0; k < length; k++)
                    {
                        // if mapped letter appears as lowercase in key
                        if (alphabet_lower[j] == key[k])
                        {
                            // calculate distance between mapped position and key position
                            dif = k - j;
                            // set encrypted character as letter in mapped position, preserving plaintext case
                            cipher = tolower(key[k - dif]);
                            printf("%c", cipher);
                            break;
                        }
                        // if mapped letter appears as uppercase in key
                        if (alphabet_upper[j] == key[k])
                        {
                            // calculate distance between mapped position and key position
                            dif = k - j;
                            // set encrypted character as letter in mapped position, preserving plaintext case
                            cipher = tolower(key[k - dif]);
                            printf("%c", cipher);
                            break;
                        }
                    }
                }
            }
        }
    }
}
