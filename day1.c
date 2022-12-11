#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef int Error;

typedef struct elv {
    int id;
    int cal;
} Elv;

int rev_comp(const void* a, const void* b)
{
    int calA = (*(Elv*) a).cal;
    int calB = (*(Elv*) b).cal;

    if (calA < calB) return 1;
    if (calA > calB) return -1;

    return 0;
}

int main(void) {

    FILE* f = fopen("./input/day1.txt", "r");
    if (!f)
    {
        fprintf(stderr, "ERROR: can't open input file.\n");
        return 1;
    }

    Elv* list = NULL;

    char line[10];
    int calSum = 0;
    int elfID = 1;

    while (fgets(line, sizeof line, f))
    {
        if (*line == '\n')
        {
            Elv* newlist = realloc(list, sizeof(Elv) * elfID);
            assert(newlist);
            list = newlist;

            list[elfID - 1].id = elfID;
            list[elfID - 1].cal = calSum;

            elfID++;
            calSum = 0;
            continue;
        }

        int cal = atoi(line);
        calSum += cal;
    }

    fclose(f);

    qsort(list, elfID, sizeof(Elv), rev_comp);

    calSum = 0;
    for (int i = 0; i < 3; ++i)
    {
        printf("Elf %d carries the most calories: %d\n", list[i].id, list[i].cal);
        calSum += list[i].cal;
    }
    printf("Callories carred by top 3: %d\n", calSum);

    return 0;
}


