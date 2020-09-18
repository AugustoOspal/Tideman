#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Numero maximo de candidatos
#define MAX 9

string candidates[MAX];

// Votos que tiene el candidato i sobre el j.
int preferences[MAX][MAX];

// Variables globales
int pair_count = 0;
int candidate_count = 0;

// Si locked[i][j] == true significa
// que el candidato i le gana al j.
bool locked[MAX][MAX];

// Cada pair tiene un perdedor y un ganador
typedef struct
{
    int winner;
    int loser;
}
pair;

pair pairs[((MAX * MAX) - MAX) / 2];

// Declaracion de funciones
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);


int main(int argc, string argv[])
{
    // Si no se ingresa ninguna info o supera el maximo ERROR.
    if (argc == 1 || argc == 10)
    {
        printf("ERROR");
        return 1;
    }

    candidate_count = argc - 1;
    int ranks[candidate_count];

    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    //#########
    //# Tally #
    //#########

    int number_of_voters = get_int("Number of voters: ");

    // Vote
    for (int i = 0; i < number_of_voters; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %d: ", j + 1);
            vote(j, name, ranks);
        }

        printf("\n");
        record_preferences(ranks);
    }

    //########################
    //# Sort, Lock and Print #
    //########################

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
}

bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = 0; j < (candidate_count - (i + 1)); j++)
        {
            preferences[ranks[i]][ranks[j + i + 1]]++;
        }

    }
}

void add_pairs(void)
{
    for (int i = 0; i < candidate_count - 1; i++)
   {
       for (int j = i + 1; j < candidate_count; j++)
       {
           if (preferences[i][j] > preferences[j][i])
           {
               pair_count++;
               pairs[pair_count - 1].winner = i;
               pairs[pair_count - 1].loser = j;
           }

           else if (preferences[i][j] < preferences[j][i])
           {
               pair_count++;
               pairs[pair_count - 1].winner = j;
               pairs[pair_count - 1].loser = i;
           }
       }
   }
}

void sort_pairs(void)
{
    // difference_between_pairs sirve para ordenar directamente
    // la resta de el ganador y la del perdedor. La resta va a estar
    // numerada con el numero al que le pertenece en pairs, luego se ordena.
    int difference_between_pairs[2][((MAX * MAX) - MAX) / 2];

    // Numerar los puestos de difference_between_pairs
    for (int i = 0; i < (((MAX * MAX) - MAX) / 2); i++)
    {
         difference_between_pairs[0][i] = i;
         difference_between_pairs[1][i] = pairs[i].winner - pairs[i].loser;
    }

    int temp;
    int swap_counter;

    // SORT
    do
    {
        swap_counter = 0;
        for (int i = 0; i < pair_count - 1; i++)
        {
            if (difference_between_pairs[1][i] < difference_between_pairs[1][i + 1])
            {
                temp = difference_between_pairs[0][i];
                difference_between_pairs[0][i] = difference_between_pairs[0][i + 1];
                difference_between_pairs[0][i + 1] = temp;

                temp = difference_between_pairs[1][i];
                difference_between_pairs[1][i] = difference_between_pairs[1][i + 1];
                difference_between_pairs[1][i + 1] = temp;

                swap_counter++;
            }
        }

    }while(swap_counter != 0);

    pair temp_p;

    for (int i = 0; i < candidate_count; i++)
    {
        temp_p.winner = pairs[i].winner;
        temp_p.loser = pairs[i].loser;
        pairs[i].winner = pairs[difference_between_pairs[0][i]].winner;
        pairs[i].loser = pairs[difference_between_pairs[0][i]].loser;
        pairs[difference_between_pairs[0][i]].winner = temp_p.winner;
        pairs[difference_between_pairs[0][i]].loser = temp_p.loser;
    }
}

void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
    }
}

void print_winner(void)
{

}