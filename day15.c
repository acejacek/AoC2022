#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INPUT_FILE "input/day15.txt"

typedef struct
{
    int x;
    int y;
    int bx;
    int by;
    long int distance;
} Sensor;

long int manhattan_distance(int p1, int p2, int q1, int q2)
{
    int dx = abs(p1 - q1);
    int dy = abs(p2 - q2);

    return (long int)dx + (long int)dy;
}

void update_distance(Sensor* s)
{
    s->distance = manhattan_distance(s->x, s->y, s->bx, s->by);
}

int main(void)
{
    Sensor* sensors = NULL;
    int counter = 0;

    int exit_status = EXIT_SUCCESS;
    const char filename[] = INPUT_FILE;
    FILE* input = fopen(filename, "r");
    if (! input)
    {
        fprintf(stderr, "Can't load file %s.\n", filename);
        exit_status = EXIT_FAILURE;
        goto cleanup;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read = 0;

    int min_x = INT_MAX;
    int max_x = INT_MIN;
    int max_distance = 0;

    for (counter = 0;(getline(&line, &len, input) != -1); ++counter)
    {
        sensors = realloc(sensors, (counter + 1) *  sizeof(Sensor));
        if (! sensors)
        {
            fprintf(stderr, "Can't allocate memory.");
            exit_status = EXIT_FAILURE;
            goto cleanup;
        }

        sscanf(line, "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d",
                &sensors[counter].x,
                &sensors[counter].y,
                &sensors[counter].bx,
                &sensors[counter].by);

        update_distance(sensors + counter);

        // calculate widely the borders, just to make sure all is analyzed
        if (min_x > sensors[counter].x) min_x = sensors[counter].x;
        if (min_x > sensors[counter].bx) min_x = sensors[counter].bx;
        if (max_x < sensors[counter].x) max_x = sensors[counter].x;
        if (max_x < sensors[counter].bx) max_x = sensors[counter].bx;
        if (max_distance < sensors[counter].distance) max_distance = sensors[counter].distance;
    }
    for (int i = 0; i < counter; ++i)
        printf("X: %d\t Y: %d\t bx: %d\t by: %d,\t dist: %ld\n",
                sensors[i].x,
                sensors[i].y,
                sensors[i].bx,
                sensors[i].by,
                sensors[i].distance);

    printf("min: %d, max %d, dist: %d\n",min_x, max_x, max_distance);

    int locations = 0;
    const int y = 2000000;
    //int y = 10;
    for (int x = min_x - max_distance; x <= max_x + max_distance; ++x)
    {
        for (int s = 0; s < counter; ++s)
        {
            // check if there is no beacon already
            if (x == sensors[s].bx && y == sensors[s].by) break;

            long int dist = manhattan_distance(x, y, sensors[s].x, sensors[s].y);
            if (sensors[s].distance >= dist)
            {
                locations++;
                break;
            }
        }
    }
    
    printf("Number of locations, where beacon cannot be present: %d\n", locations);

    //const int limit = 20;
    const int limit = 4000000;
    for (int x = 0; x <= limit; ++x)
    {
        for (int y = 0; y <= limit; ++y)
        {
            int good = 1;
            for (int s = 0; s < counter; ++s)
            {
                // check if there is no beacon already
                if (x == sensors[s].bx && y == sensors[s].by)
                {
                    good = 0;
                    break;
                }

                long int dist = manhattan_distance(x, y, sensors[s].x, sensors[s].y);

                if (sensors[s].distance >= dist)
                {
                    good = 0;
                    break;
                }
            }
            if (good)
            {
                printf("Good location X: %d Y: %d tuning: %d\n", x, y, 4000000 * x + y);
                goto cleanup;
            }
        }
    }

cleanup:
    if (input) fclose(input);
    free(sensors);
    exit(exit_status);
}

