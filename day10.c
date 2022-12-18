#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

void drawPixel(int cycle, int regX, char* screen)
{
    int x = cycle % 40;
    int y = cycle / 40;

    if (x - 1 == regX || x == regX || x + 1 == regX)
    {
        screen[y * 40 + x] = 1;
    }
}

int addCycle(int cycle, int regX, char* screen)
{
    static int accumulator = 0;

    drawPixel(cycle, regX, screen);

    switch (++cycle)
    {
        case 20:
            accumulator = cycle * regX;
            break;

        case 60:
            accumulator += cycle * regX;
            break;

        case 100:
            accumulator += cycle * regX;
            break;

        case 140:
            accumulator += cycle * regX;
            break;

        case 180:
            accumulator += cycle * regX;
            break;

        case 220:
            accumulator += cycle * regX;
            printf("Signal strength: %d\n", accumulator);
            break;

        default:
            break;
    }

    return cycle;
}

void printScreen(char* screen)
{
    for (int i = 0; i < 240; ++i)
    {
        if (i % 40 == 0) printf("\n");

        printf("%c", screen[i] ? '#' : ' ');
    }

    printf("\n");
}

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

int main(void)
{
    FILE* f = openFile("./input/day10.txt");

    int cycle = 0;
    int regX = 1;

    char screen[240] = { 0 };

    char line[20];

    while (fgets(line, 20, f) != NULL)
    {
        char instruction[5];
        sscanf(line, "%s", instruction);

        if (instruction[0] == 'n')  // noop
        {
            cycle = addCycle(cycle, regX, screen);
            continue;
        }

        // addv
        int value;
        sscanf(line, "%s %d", instruction, &value);

        cycle = addCycle(cycle, regX, screen);
        cycle = addCycle(cycle, regX, screen);

        regX += value;
    }

    printScreen(screen);

    fclose(f);
    return 0;
}
