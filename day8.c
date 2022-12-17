#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define INPUT_WIDTH 200

typedef struct tree {
    int height;
    bool visible;
    int scenicScore;
    int x1;
    int x2;
    int y1;
    int y2;
} Tree;

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


void printForest(Tree* f, int size, int width)
{
    for (int i = 0; i < size; ++i)
    {
        if (i % width == 0) printf("\n");
        printf("%d", f[i].height);
        if (!f[i].visible)
            printf("'");
        else
            printf(" ");
    }
    printf("\n\n");
}


void setVisibility(Tree* f, int size, int width)
{
    int rows = size / width;
    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0 ; x < width ; ++x)
        {
            // horizontally
            if (y == 0 || x == 0 || y == rows - 1 || x == width - 1)
            {
                f[y * width + x].visible = true;
                continue;
            }

            bool higherFound = false;
            for (int i = 0; i < x; ++i)
            {
                if (f[y * width + i].height >= f[y * width + x].height)   // there is a higher tree to the left
                {
                    higherFound = true;
                    break;
                }
            }
            if (!higherFound)
            {
                f[y * width + x].visible = true;
                continue;
            }

            higherFound = false;

            for (int i = x + 1; i < width; ++i)
            {
                if (f[y * width + i].height >= f[y * width + x].height)   // there is a higher tree to the right
                {
                    higherFound = true;
                    break;
                }
            }
            if (!higherFound)
            {
                f[y * width + x].visible = true;
                continue;
            }

            // vertically

            higherFound = false;
            for (int i = 0; i < y; ++i)
            {
                if (f[i * width + x].height >= f[y * width + x].height)   // higher above
                {
                    higherFound = true;
                    break;
                }
            }
            if (!higherFound)
            {
                f[y * width + x].visible = true;
                continue;
            }

            higherFound = false;
            for (int i = y + 1; i < rows; ++i)
            {
                if (f[i * width + x].height >= f[y * width + x].height)   // higher below
                {
                    higherFound = true;
                    break;
                }
            }

            if (!higherFound)
            {
                f[y * width + x].visible = true;
            }
        }
    }
}

int countVisible(Tree* f, int size)
{
    int res = 0;

    for (int i = 0; i < size; ++i)
        if (f[i].visible) res++;

    return res;
}

int bestScenic(Tree* f, int size)
{
    int res = 0;

    for (int i = 0; i < size; ++i)
        if (res < f[i].scenicScore) res = f[i].scenicScore;

    return res;
}
void calculateScenicScore(Tree* f, int size, int width)
{
    int rows = size / width;
    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0 ; x < width ; ++x)
        {
            for (int i = x - 1; i >= 0; --i)   // left
            {
                f[y * width + x].x1++;

                if (f[y * width + i].height >= f[y * width + x].height) break;
            }

            for (int i = x + 1; i < width; ++i)   // right
            {
                f[y * width + x].x2++;

                if (f[y * width + i].height >= f[y * width + x].height) break;
            }

            for (int i = y - 1; i >= 0; --i)   // up
            {
                f[y * width + x].y1++;

                if (f[i * width + x].height >= f[y * width + x].height) break;
            }

            for (int i = y + 1; i < rows; ++i)   // down
            {
                f[y * width + x].y2++;

                if (f[i * width + x].height >= f[y * width + x].height) break;
            }

            f[y * width + x].scenicScore =
                f[y * width + x].x1 *
                f[y * width + x].x2 *
                f[y * width + x].y1 *
                f[y * width + x].y2;
        }
    }
}

int main(void)
{
    FILE* f = openFile("./input/day8.txt");

    Tree* forest = NULL;
    int width = 0;
    int size = 0;
    char line[INPUT_WIDTH];

    while (fgets(line, INPUT_WIDTH, f) != NULL)
    {
        width = strlen(line) - 1;  // ignore EOL
        forest = realloc(forest, sizeof(Tree) * (size + width));
        assert(forest);

        for (int i = 0; i < width; ++i)
        {
            forest[size + i].height = (int)line[i]-'0';
            forest[size + i].visible = false;
            forest[size + i].x1 = 0;
            forest[size + i].x1 = 0;
            forest[size + i].x2 = 0;
            forest[size + i].y1 = 0;
            forest[size + i].y2 = 0;
        }

        size += width;
    }

    fclose(f);

    setVisibility(forest, size, width);
    calculateScenicScore(forest, size, width);
//    printForest(forest, size, width);

    int visible = countVisible(forest, size);
    int scenic = bestScenic(forest, size);

    printf("Trees visible in the forest: %d\n", visible);
    printf("Best scenic score is: %d\n", scenic);

    return 0;
}
