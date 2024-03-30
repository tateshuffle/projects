// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// create root node
node *root = NULL;

// keep track of words in hash table
int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // hash word
    int index = hash(word);

    // create pointer to root node
    node *trav = table[index];

    // compare word in node with dictionary
    while (trav != NULL)
    {
        if (strcasecmp(trav->word, word) == 0)
        {
            return true;
        }
        else
        {
            trav = trav->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int sum = 0;
    for (int i = 0; word[i] != '\0'; i++)
    {
        sum += toupper(word[i]) - 'A';
    }
    return sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // open dictionary file
    FILE *source = (fopen(dictionary, "r"));
    if (source == NULL)
    {
        return false;
    }

    // create buffer for each word in dictionary
    char *word_buffer = malloc((LENGTH + 1) * sizeof(char));
    if (word_buffer == NULL)
    {
        return false;
    }

    // initialize table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // read each word on file
    while (fscanf(source, "%s", word_buffer) != EOF)
    {
        // hash word
        int index = hash(word_buffer);

        // increase word count
        word_count++;

        // create node
        node *copy = malloc(sizeof(node));
        if (copy == NULL)
        {
            return false;
        }
        // populate node
        strcpy(copy->word, word_buffer);
        copy->next = table[index];

        // add to table
        table[index] = copy;
    }
    free(word_buffer);
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // loop through table
    for (int i = 0; i < N; i++)
    {
        // set pointer to current table element
        node *cursor = table[i];

        // free each node in the list of current hash code
        while (cursor != NULL)
        {

            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
