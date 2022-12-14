#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

// 4 for part 1, 14 for part 2:
#define DISTLIMIT 14

char dataBuffer[DISTLIMIT] = {0};

FILE* openFile(const char* filename)
{
    FILE* f = fopen(filename, "r");
    if (!f)
    {
        fprintf(stderr, "ERROR: can't open input file.\n");
        exit(1);
    }
    return f;
}

void insertIntoBuffer(char a)
{
    for (int i = 0; i < DISTLIMIT; ++i)
        dataBuffer[i] = dataBuffer[i + 1];

    dataBuffer[DISTLIMIT - 1] = a;
}

/*
void printBuffer()
{
    for (int i = 0; i < DISTLIMIT; ++i)
        printf("%c ",dataBuffer[i]);

    printf("\n");
}
*/

bool isBufferUnique()
{
    if (dataBuffer[0] == 0) return false;     // buffer partially empty

    for (int i = 0; i < DISTLIMIT - 1; ++i)
    {
        for (int j = i + 1; j < DISTLIMIT; ++j)
        {
            if (dataBuffer[i] == dataBuffer[j]) return false;
        }
    }

    return true;
}

int main(void)
{
    FILE* f = openFile("./input/day6.txt");

    char c;
    int counter = 0;
    bool found = false;

    while ((c = fgetc(f)) != EOF)
    {
        counter++;
        insertIntoBuffer(c);
//        printBuffer();
        if (isBufferUnique())
        {
            found = true;
            break;
        }
    }
    fclose(f);

    if (found)
        printf("Marker found at position %d\n", counter);
    else
        printf("Marker not found.\n");

    return 0;
}

