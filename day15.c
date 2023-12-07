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
    int distance;
} Sensor;

int manhattan_distance(const int p1, const int p2, const int q1, const int q2)
{
    int dx = abs(p1 - q1);
    int dy = abs(p2 - q2);

    return dx + dy;
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

    int locations = 0;
    const int y = 2000000;
    //int y = 10;
    for (int x = min_x - max_distance; x <= max_x + max_distance; ++x)
        for (int s = 0; s < counter; ++s)
        {
            // check if there is no beacon already
            if (x == sensors[s].bx && y == sensors[s].by) break;

            int dist = manhattan_distance(x, y, sensors[s].x, sensors[s].y);
            if (sensors[s].distance >= dist)
            {
                locations++;
                break;
            }
        }
    
    printf("Number of locations, where beacon cannot be present: %d\n", locations);

    const int limit = 4000000;
    for (int x = 0; x <= limit; ++x)
        for (int y = 0; y <= limit; ++y)
        {
            int good = 1;
            for (int s = 0; s < counter; ++s)
            {
                int dist = manhattan_distance(x, y, sensors[s].x, sensors[s].y);
                if (sensors[s].distance >= dist)
                {
                    // I'm inside the ball
                    good = 0;
                    if (y < sensors[s].y) // I'm left to the sensor
                    {
                        // I can already skip right to mirror position
                        y += 2 * (sensors[s].y - y);
                    }
                    // now I'm right to the sensor
                    // add diff of distances to stand on right outer edge of ball
                    y += sensors[s].distance - dist;

                    // no need to check other sensors
                    break;
                }
            }
            if (good) // my distance is larger than balls of all sensors
            {
                printf("Good location X: %d Y: %d; Tuning: %zu\n", x, y, (size_t) 4000000 * x + y);
                goto cleanup;
            }
        }

cleanup:
    if (input) fclose(input);
    free(sensors);
    exit(exit_status);
}

