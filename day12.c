#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>

struct point
{
    char elevation;
    bool visited;
    char picture;
    int distance;
    int previous;
};

typedef struct map
{
    struct point* data;
    int size;
    int width;
    int height;
} Map;

void printMap(Map* m)
{
    for (int i = 0; i < m->size; ++i)
    {
        if (i % m->width == 0) printf("\n");
        putchar(m->data[i].picture);
    }
    printf("\n");
}

void examineNeighbour(Map* m, int winner, int target)
{
    if (m->data[target].visited) return;

    if (m->data[target].elevation > m->data[winner].elevation + 1) return;  // target to high

    int newDistance = m->data[winner].distance + 1;

    if (m->data[target].distance > newDistance)
    {
        m->data[target].distance = newDistance;
        m->data[target].previous = winner;
    }
}

void findRouteDijkstra(Map* m)
{
    int winner = 0;
    bool visiting;

    do {
        visiting = false;

        int smallest = INT_MAX;
        for (int i = 0; i < m->size; ++i)
        {
            if (m->data[i].visited) continue;

            if (m->data[i].distance < smallest)
            {
                winner = i;
                smallest = m->data[i].distance;
                visiting = true;
            }
        }

        if (m->data[winner].picture == 'E') break;  // found exit

        if (!visiting) break;  // all nodes have been reviewed

        m->data[winner].visited = true;

        int x = winner % m->width;
        int y = winner / m->width;

        if (x < m->width - 1)
            examineNeighbour(m, winner, winner + 1);

        if (x > 0)
            examineNeighbour(m, winner, winner - 1);

        if (y < m->height - 1)
            examineNeighbour(m, winner, winner + m->width);

        if (y > 0)
            examineNeighbour(m, winner, winner - m->width);

    } while (true);
}

int countRoute(Map* m)
{
    // find End
    int i = 0;
    while (m->data[i].picture != 'E') i++;

    // walk backwards to start
    int step = 0;
    for (;m->data[i].picture != 'S'; ++step)
    {
        if (m->data[i].distance == INT_MAX) return -1;  // start not found
        i = m->data[i].previous;
    }
    return step;
}

void resetMap(Map* m)
{
    for (int i = 0; i < m->size; ++i)
    {
        m->data[i].distance = INT_MAX;
        m->data[i].visited = false;
        if (m->data[i].picture == 'S') m->data[i].picture = 'a';
    }
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
    FILE* f = openFile("./input/day12.txt");

    Map map = {
        .data = NULL,
        .size = 0,
        .width = 0,
        .height = 0,
    };

    char c;
    while ((c = fgetc(f)) != EOF)
    {
        if (c == '\n') {
            if (!map.width) map.width = map.size;
            map.height++;
            continue;
        }

        map.size++;
        map.data = realloc(map.data, map.size * sizeof *map.data);
        assert(map.data);

        map.data[map.size - 1].elevation = c;
        map.data[map.size - 1].picture = c;
        map.data[map.size - 1].visited = false;
        map.data[map.size - 1].distance = INT_MAX;

        if (c == 'E')
            map.data[map.size - 1].elevation = 'z';

        if (c == 'S')
        {
            map.data[map.size - 1].elevation = 'a';
            map.data[map.size - 1].distance = 0;
        }
    }
    fclose(f);

    printMap(&map);

    findRouteDijkstra(&map);

    printf("Shortest walk from S to E takes %d steps.\n", countRoute(&map));

    // part 2

    int shortestPossible = INT_MAX;

    resetMap(&map);

    // scan all possible starting points
    for (int i = 0; i < map.size; ++i)
    {
        if (map.data[i].elevation != 'a') continue;

        map.data[i].picture = 'S';
        map.data[i].distance = 0;

        findRouteDijkstra(&map);

        int distance = countRoute(&map);

        if (distance < 0) goto reset;     // no way out. probably does not need resetting the map

        if (shortestPossible > distance)
        {
            shortestPossible = distance;
            putchar('+');
        }
        else
            putchar('.');

        fflush(stdout);
reset:
        resetMap(&map);
    }

    printf("\n\nShortest walk from closest 'a' to E is %d steps.\n", shortestPossible);

    return 0;
}

