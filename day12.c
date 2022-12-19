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
    bool startTested;
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
    for (int y = 0; y < m->height; ++y)
    {
        for (int x = 0; x < m->width; ++x)
        {
            printf("%c", m->data[y * m->width + x].picture);
        }
        printf("\n");
    }
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
    while (m->data[i].picture != 'S')
    {
        if (m->data[i].distance == INT_MAX) return -1;  // start not found
        step++;
        i = m->data[i].previous;
        //printf("%d ", m->data[i].distance);
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
        map.data[map.size - 1].startTested = false;

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

    printf("Shortest walk from S to E took %d steps.\n", countRoute(&map));

    // part 2

    int shortestPossible = INT_MAX;

    bool allStartsExamined = false;

    while (!allStartsExamined)
    {
        allStartsExamined = true;

        resetMap(&map);

        // find starting point
        for (int i = 0; i < map.size; ++i)
        {
            if (map.data[i].elevation != 'a') continue;
            if (map.data[i].startTested) continue;

            allStartsExamined = false;

            map.data[i].startTested = true;
            map.data[i].picture = 'S';
            map.data[i].distance = 0;

            findRouteDijkstra(&map);

            int distance = countRoute(&map);
            if (distance < 0)
            {
                putc('x', stdout);
                fflush(stdout);
                break;
            }

            if (shortestPossible > distance)
            {
                shortestPossible = distance;
                putc('+', stdout);
            }
            else putc('.', stdout);

            fflush(stdout);

            break;
        }
    }

    printf("\n\nShortest walk from closest 'a' to E is %d steps.\n", shortestPossible);

    return 0;
}

