#include <cs50.h>
#include <stdio.h>
#include <strings.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // create ballot for each voter
    for (int ballot = voter; ballot < voter_count; ballot++)
    {
        // loop through candidate list
        for (int j = 0; j < candidate_count; j++)
        {
            // compare input to each name
            if (strcasecmp(name, candidates[j].name) == 0)
            {
                // place index in rank position
                preferences[ballot][rank] = j;
                return true;
            }
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // loop throug voters
    for (int ballot = 0; ballot < voter_count; ballot++)
    {
        // loop through candidates
        for (int index = 0; index < candidate_count; index++)
        {
            // count votes for first position
            if (preferences[ballot][0] == index && candidates[index].eliminated == false)
            {
                candidates[index].votes++;
                break;
            }
            else if (preferences[ballot][0] == index && candidates[index].eliminated == true)
            {
                // count votes for second position
                for (int next = 0; next < candidate_count; next++)
                {
                    if (preferences[ballot][1] == next)
                    {
                        candidates[next].votes++;
                        break;
                    }
                    else
                    {
                        // look for next available choice
                        for (int runners = 0; runners < candidate_count; runners++)
                        {
                            if (candidates[runners].eliminated == false)
                            {
                                // move to second position
                                preferences[ballot][1] = runners;
                            }
                        }
                    }
                }
            }
        }
    }
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // set threshold for more than half of total votes
    int half = (voter_count / 2) + 1;

    // loop through candidate list
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes == half)
        {
            printf("winner: %s\n", candidates[i].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min;
    for (int i = 0; i < candidate_count; i++)
    {
        // set initial minimum value
        min = candidates[0].votes;

        // check if candidate is eliminated
        if (candidates[i].eliminated == true)
        {
            // set eliminated candidate as starting value
            candidates[i] = candidates[0];
            // adjust counter and array size removing eliminated candidate
            for (int j = 1; j < candidate_count - 1; j++)
            {
                // compare amount of votes for remaining candidates
                if (candidates[j].votes < candidates[0].votes)
                {
                    min = candidates[j].votes;
                }
            }
        }

        // compare amount of votes for each candidate
        if (candidates[i].votes < candidates[0].votes)
        {
            min = candidates[i].votes;
        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // initialize variable to count how many are tied
    int tied_candidates = 0;

    for (int i = 0; i < candidate_count; i++)
    {
        // check if candidate is eliminated
        if (candidates[i].eliminated == true)
        {
            // adjust counter and array size removing eliminated candidate
            for (int j = 1; j < candidate_count - 1; j++)
            {
                // look for candidates with minimum number of votes
                if (candidates[j].votes == min)
                {
                    tied_candidates++;
                }
                if (tied_candidates == candidate_count - 1)
                {
                    return true;
                }
            }
        }

        // look for tie when all remain
        if (candidates[i].votes == min)
        {
            tied_candidates++;
        }

        if (tied_candidates == candidate_count)
        {
            return true;
        }
    }
    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        // check if candidate is in last place
        if (candidates[i].votes == min)
        {
            candidates[i].eliminated = true;
        }
    }
    return;
}