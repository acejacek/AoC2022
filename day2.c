#include <stdio.h>
#include <stdlib.h>

int score(char elf, char player)
{
    int score = 0;
    if (player == 'A') score = 1; // rock
    if (player == 'B') score = 2; // paper
    if (player == 'C') score = 3; // scisors

    if (elf == player) return score + 3;   // draw

    if (elf == 'A' && player == 'B') return score + 6; // rock - paper
    if (elf == 'A' && player == 'C') return score ;    // rock - scisors

    if (elf == 'B' && player == 'A') return score;     // paper -  rock
    if (elf == 'B' && player == 'C') return score + 6; // paper - scisors

    if (elf == 'C' && player == 'A') return score + 6; // scisors - rock
    if (elf == 'C' && player == 'B') return score;     // scisors - paper

    return score;
}

int main(void)
{
    FILE* f = fopen("./input/day2.txt", "r");
    if (!f)
    {
        fprintf(stderr, "ERROR: can't open input file.\n");
        return 1;
    }

    char action;
    char response;

    int score1 = 0;
    int score2 = 0;

    do {
        int err = fscanf(f, "%c %c\n", &action, &response);
        if (err == EOF) break;

        char newResponse;
        if (response == 'X') newResponse = 'A';
        if (response == 'Y') newResponse = 'B';
        if (response == 'Z') newResponse = 'C';

        score1 += score(action, newResponse);

        if (response == 'X') {  // loose
            if (action == 'A') newResponse = 'C';
            if (action == 'B') newResponse = 'A';
            if (action == 'C') newResponse = 'B';
        }

        if (response == 'Y') {  // draw
            newResponse = action;
        }

        if (response == 'Z') {  // win
            if (action == 'A') newResponse = 'B';
            if (action == 'B') newResponse = 'C';
            if (action == 'C') newResponse = 'A';
        }

        score2 += score(action, newResponse);

    } while (1);

    printf("Score 1: %d\n", score1);
    printf("Score 2: %d\n", score2);

    return 0;
}
