#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcasecmp(name, candidates[i].name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    // variable to keep track of candidate of most votes
    candidate winner;

    // iterate through array of candidates checking amount of votes
    for (int i = 0; i < candidate_count; i++)
    {
        // compare each candidates' amount of votes with the first candidate's votes
        if (candidates[i].votes > candidates[0].votes)
        {
            winner = candidates[i];
            candidates[i] = candidates[0];
            // replace first element with biggest amount of votes
            candidates[0] = winner;
        }
    }
    printf("%s\n", candidates[0].name);

    // check for ties
    for (int j = 1; j < candidate_count; j++)
    {
        if (candidates[j].votes == candidates[0].votes)
        {
            // declare candidates with same amount of votes as winners
            printf("%s\n", candidates[j].name);
        }
    }
    return;
}