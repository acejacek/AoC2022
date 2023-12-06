#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH 1000
#define HEIGHT 200

#define INPUT_FILE "input/day14.txt"

#define PART 1

void plot(char* area, const int x, const int y, const char block)
{
    if (y * WIDTH + x > WIDTH * HEIGHT) return; // out of bounds
    area[y * WIDTH + x] = block;
}

char peek(char* area, const int x, const int y)
{
    if (y * WIDTH + x > WIDTH * HEIGHT) return '#'; // out of bounds
    return area[y * WIDTH + x];
}

// [XXX] carefully with parameters, no bound check!
void draw_area(char* area, const int x, const int y, const int width, const int height)
{
    for (int i = y; i < y + height; ++i)
    {
        for (int j = x; j < x + width; ++j)
        {
            char c = peek(area, j, i);
            if (c == '\0') c = '.';
            putchar(c);
        }
        putchar('\n');
    }
}

int drop_sand(char* area, const int x, const int y, size_t* count )
{
    if (y == HEIGHT) return 1; // hit bottom
    
    if (peek(area, x, y)) return 0; // full

    // is free below? 
    if (drop_sand(area, x, y + 1, count)) return 1;

    // is free below & left? 
    if (drop_sand(area, x - 1, y + 1, count)) return 1;

    // is free below & right? 
    if (drop_sand(area, x + 1, y + 1, count)) return 1;

    plot(area, x, y, 'o');
    (*count)++;

    return 0;
}

int main(void)
{
    int exit_status = EXIT_SUCCESS;
    char* area = NULL;
    const char filename[] = INPUT_FILE;
    FILE* input = fopen(filename, "r");
    if (! input)
    {
        fprintf(stderr, "Can't load file %s.\n", filename);
        exit_status = EXIT_FAILURE;
        goto cleanup;
    }

    area = calloc(WIDTH * HEIGHT, sizeof(char));
    if (! area)
    {
        fprintf(stderr, "Can't allocate memory.");
        exit_status = EXIT_FAILURE;
        goto cleanup;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    int max_y = 0;
   
    while ((read = getline(&line, &len, input)) != -1)
    {
        char* pair = strtok(line, " ->");

        int x1, y1;
        sscanf(pair, "%d,%d", &x1, &y1);

        while (pair != NULL)
        {
            int x2, y2;
            sscanf(pair, "%d,%d", &x2, &y2);

            // rudimentary check
            if (x2 + 2 > WIDTH) printf("x value %d  out of bound\n", x2);
            if (y2 + 2 > HEIGHT) printf("y value %d out of bound\n", y2);

            if (x1 == x2) // vertical wall
            {
                for (int i = y1; i != y2;)
                {
                    plot(area, x1, i, '#');
                    if (y1 < y2) i++;
                    else i--;
                }
                plot(area, x1, y2, '#');
            }
            else  // horizontal wall
            {
                for (int i = x1; i != x2;)
                {
                    plot(area, i, y1, '#');
                    if (x1 < x2) i++;
                    else i--;
                }
                plot(area, x2, y1, '#');
            }

            // for next wall element
            x1 = x2;
            y1 = y2;

            // needed for part 2 only
            if (max_y < y1) max_y = y1;
            if (max_y < y2) max_y = y2;

            pair = strtok(NULL, " ->");
        }
    }
#if PART == 2
        for (int i = 0; i < WIDTH; ++i)
            plot(area, i, max_y + 2, '#');
#endif

    size_t count = 0;
    drop_sand(area, 500, 0, &count);

//    draw_area(area, 450, 0, 100, 50);

    printf("Sand count: %zu\n", count);

cleanup:
    if (input) fclose(input);
    free(area);
    exit(exit_status);
}

