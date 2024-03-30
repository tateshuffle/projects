#include <cs50.h>
#include <stdio.h>
#include <strings.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool cycle(int loser, int winner);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // loop through candidates array
    for (int index = 0; index < candidate_count; index++)
    {
        // compare input to name in each position of array
        if (strcasecmp(name, candidates[index]) == 0)
        {
            // set candidate index as rank value in ranks array
            ranks[rank] = index;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // loop through ranked candidates
    for (int position = 0; position < candidate_count; position++)
    {
        // match with candidate in next rank
        for (int next = position + 1; next < candidate_count; next++)
        {
            // increease value of advantage in preferences matrix
            preferences[ranks[position]][ranks[next]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int match, reverse;

    // loop through rows in preferences matrix
    for (int i = 0; i < candidate_count; i++)
    {
        // match with columns
        for (int j = 0; j < candidate_count; j++)
        {
            // compare value of preferences for each pair
            match = preferences[i][j];
            reverse = preferences[j][i];

            // select winning version of pair
            if (match > reverse)
            {
                // create new pair
                pair_count++;

                // set pair's winner and loser
                for (int k = 0; k <= pair_count; k++)
                {
                    pairs[k].winner = i;
                    pairs[k].loser = j;
                }
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair temp;

    // loop through pairs
    for (int i = 0; i <= pair_count; i++)
    {
        // look at next pair
        for (int j = i + 1; j <= pair_count; j++)
        {
            // place bigger advantages in the beginning of pairs array
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[j].winner][pairs[j].loser])
            {
                temp = pairs[i];
                pairs[i] = pairs[j];
                pairs[j] = temp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // loop through pairs array
    for (int i = 0; i < pair_count; i++)
    {
        // check for cycles
        if (!cycle(pairs[i].loser, pairs[i].winner))
        {
            // lock pairs that don't create cycle
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// check if locking a pair will create a cycle
bool cycle(int loser, int winner)
{
    // base case = candidate in losing position points at itself in winning position
    if (loser == winner)
    {
        return true;
    }
    // loop through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        // look for possible chains checking if there's any locked edge from loser to current candidate
        if (locked[loser][i])
        {
            // if chain is possible, call function again using current candidate as loser to check for cycle
            if (cycle(i, winner))
            {
                return true;
            }
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // loop through rows in locked matrix
    for (int i = 0; i < candidate_count; i++)
    {
        // initialize variable to count false values
        int false_count = 0;
        // loop through columns in locked matrix
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                false_count++;
                // column with only false values is source of graph
                if (false_count == candidate_count)
                {
                    printf("%s\n", candidates[i]);
                }
            }
        }
    }
    return;
}