#include <cs50.h>
#include <stdio.h>
#include <string.h>

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
} pair;

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
    // Loop through each candidate, where i is the index
    for (int i = 0; i < candidate_count; i++)
    {
        // if the name on the vote is found
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Loop through each candidate, where i is the rank
    for (int i = 0; i < candidate_count; i++)
    {
        // Loop through each candidate, where j is the rank lower than i
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Compare all possible combinations of candidates
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // Comparison does not continue if it is the same candidate
            if (i == j)
            {
                break;
            }
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Bubble sort
    for (int i = 0; i < pair_count - 1; i++)
    {
        for (int j = 0; j < pair_count - 2; j++)
        {
            int strengths[2];
            strengths[0] = pairs[j].winner - pairs[j].loser;
            strengths[1] = pairs[j + 1].winner - pairs[j + 1].loser;
            if (strengths[0] > strengths[1])
            {
                pair temp = pairs[j];
                pairs[j] = pairs[j + 1];
                pairs[j + 1] = temp;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Loop through pairs
    for (int i = 0; i < pair_count; i++)
    {
        int winner = pairs[i].winner;
        int loser = pairs[i].loser;

        // Check if it will create a cycle with Depth First Search (DFS) Algorithm
        bool has_cycle = false;
        // List if [i] candidate was visited
        bool visited[candidate_count];
        for (int j = 0; j < candidate_count; j++)
        {
            visited[j] = false;
        }
        visited[winner] = true;
        // List which candidate will be visited
        int stack[candidate_count];
        int stack_length = 0;

        // Push loser into stack
        stack[stack_length] = loser;
        stack_length++;

        while (stack_length > 0 && has_cycle == false)
        {
            int target = stack[0];
            // Check if there is a cycle
            if (visited[target] == true)
            {
                has_cycle = true;
            }
            else
            {
                // Mark as visited
                visited[target] = true;

                // Update stack
                stack_length--;
                for (int j = 0; j < stack_length; j++)
                {
                    stack[j] = stack[j + 1];
                }

                // Look for "children" aka "edges"
                for (int k = 0; k < candidate_count; k++)
                {
                    if (locked[target][k])
                    {
                        stack[stack_length] = k;
                        stack_length++;
                    }
                }
            }
        }

        // Add edge if not creating a cycle
        if (has_cycle == false)
        {
            locked[winner][loser] = true;
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    // Look who doesn't receive an arrow
    for (int j = 0; j < candidate_count; j++)
    {
        int winner = j;
        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[i][j])
            {
                winner = -1;
            }
        }

        if (winner == j)
        {
            printf("%s\n", candidates[j]);
            return;
        }
    }
}