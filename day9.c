#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

// for first task set to 2
// for second part set to 10
#define KNOTS 10

#define WIDTH 500
#define HEIGHT 500

typedef struct {
    int x;
    int y;
} RopeEnd;

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

int countTraces(unsigned char* field)
{
    int result = 0;
    for (int i = 0; i < WIDTH * HEIGHT; ++i)
        if (field[i]) result++;

    return result;
}

void printField(unsigned char* field)
{
    for (int y = 0; y < HEIGHT; ++y)
    {
        for (int x = 0; x < WIDTH; ++x)
        {
           printf("%c", field[y * WIDTH + x] ? '#' : '-');
        }

        printf("\n");
    }
}

void markTrail(RopeEnd e, unsigned char* field)
{
    size_t address = e.y * WIDTH + e.x;

    if (address > WIDTH * HEIGHT)
    {
        fprintf(stderr, "Error: Field to small!\n");
        exit(1);
    }

    field[address] = 1;
}

RopeEnd dragTail(RopeEnd h, RopeEnd t)
{
    if (abs(h.x - t.x) < 2 && abs(h.y - t.y) < 2) return t; // too close

    RopeEnd newTail = t;

    if (h.x > t.x) newTail.x++;
    else if (h.x < t.x) newTail.x--;

    if (h.y > t.y) newTail.y++;
    else if (h.y < t.y) newTail.y--;

    return newTail;
}

void dragKnots(RopeEnd* knots, unsigned char* field)
{
    for (int i = 0; i < KNOTS - 1; ++i)
    {
        knots[i + 1] = dragTail(knots[i], knots[i + 1]);
    }

    markTrail(knots[KNOTS - 1], field);
}

int main(void)
{
    FILE* f = openFile("./input/day9.txt");

    unsigned char* field = calloc(WIDTH * HEIGHT, sizeof *field );
    assert(field);

    RopeEnd knots[KNOTS];

    for (int i = 0; i < KNOTS; ++i)
        knots[i] = (RopeEnd){WIDTH / 2, HEIGHT / 2};

    markTrail(knots[KNOTS - 1], field);

    char dir;
    int steps;

    while (fscanf(f, "%c%d\n", &dir, &steps) > 0)
    {
        switch (dir) {

            case 'U':
                for (int i = 0; i < steps; ++i)
                {
                    knots[0].y--;
                    dragKnots(knots, field);
                }
                break;

            case 'D':
                for (int i = 0; i < steps; ++i)
                {
                    knots[0].y++;
                    dragKnots(knots, field);
                }
                break;

            case 'L':
                for (int i = 0; i < steps; ++i)
                {
                    knots[0].x--;
                    dragKnots(knots, field);
                }
                break;

            case 'R':
                for (int i = 0; i < steps; ++i)
                {
                    knots[0].x++;
                    dragKnots(knots, field);
                }
                break;

            default:
                break;
        }
    }

    fclose(f);

    printField(field);

    printf("Tail of the rope visited %d places.\n", countTraces(field));

    free(field);
    return 0;
}
