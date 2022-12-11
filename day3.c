#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int prio(char a)
{
    if (a > 'Z') return a - 'a' + 1;

    return a - 'A' + 27;
}

char common(char* a, char* b)
{
    int limit = strlen(a);
    for (int i = 0; i < limit; ++i) {
        for (int j = 0; j < limit; ++j) {
            if (a[i] == b[j]) return a[i];
        }
    }
    return 0;
}

char common3(char* a, char* b, char* c)
{
    for (size_t i = 0; i < strlen(a); ++i) {
        for (size_t j = 0; j < strlen(b); ++j) {
            if (a[i] == b[j]) {
                for (size_t k = 0; k < strlen(c); ++k) {
                    if (b[j] == c[k]) return c[k];
                }
            }
        }
    }
    return 0;
}

FILE* openFile(void)
{
    FILE* f = fopen("./input/day3.txt", "r");
    if (!f)
    {
        fprintf(stderr, "ERROR: can't open input file.\n");
        exit(1);
    }
    return f;
}

int main(void)
{
    FILE* f;

    f = openFile();

    int prioritySum = 0;

    char line[100];
    while (fscanf(f, "%s", line) != EOF)
    {
        int mid = strlen(line) >> 1;

        char a[100];
        strncpy(a, line, mid);
        a[mid] = '\0';

        char b[100];
        strncpy(b, line + mid, 100);

        char res = common(a, b);
        assert(res != 0);

        prioritySum += prio(res);
    }

    fclose(f);

    printf("Total priority : %d\n", prioritySum);

    //
    // Second task
    //

    f = openFile();

    prioritySum = 0;

    char elf1[100];
    char elf2[100];
    char elf3[100];
    while (fscanf(f, "%s", elf1) != EOF)
    {
        fscanf(f, "%s", elf2);
        fscanf(f, "%s", elf3);

        char res = common3(elf1, elf2, elf3);
        assert(res != 0);

        prioritySum += prio(res);
    }

    fclose(f);

    printf("Total priority of badges: %d\n", prioritySum);

    return 0;
}

