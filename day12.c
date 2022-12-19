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
    int x;
    int y;
    int shortestTrip;
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

void printDistances(Map* m)
{
    for (int y = 0; y < m->height; ++y)
    {
        for (int x = 0; x < m->width; ++x)
        {
            printf("%d\t", m->data[y * m->width + x].distance);
        }
        printf("\n");
    }
}

void findStart(Map* m)
{
    for (int y = 0; y < m->height; ++y)
    {
        for (int x = 0; x < m->width; ++x)
        {
            if (m->data[y * m->width + x].picture == 'S')
            {
                m->x = x;
                m->y = y;
                return;
            }
        }
    }

    fprintf(stderr, "Start not found\n");
    exit(1);
}

void findRouteBruteforce(Map* m, int step)
{
    int address = m->y * m->width + m->x;
    char myElevation = m->data[address].elevation;
    if (m->data[address].picture == 'E')
    {
        if (!m->shortestTrip || m->shortestTrip > step) m->shortestTrip = step;
        return;
    }

    if (m->shortestTrip && step > m->shortestTrip) return;  // this is already longer than best score

    m->data[address].visited = true;

    if (m->x < m->width -1)
    {
        if (!m->data[address + 1].visited)
        {
            if (m->data[address + 1].elevation <= myElevation + 1)
            {
                m->x++;
                findRouteBruteforce(m, step + 1);
                m->x--;
            }
        }
    }

    if (m->x > 0)
    {
        if (!m->data[address - 1].visited)
        {
            if (m->data[address - 1].elevation <= myElevation + 1)
            {
                m->x--;
                findRouteBruteforce(m, step + 1);
                m->x++;
            }
        }
    }

    if (m->y > 0)
    {
        if (!m->data[address - m->width].visited)
        {
            if (m->data[address - m->width].elevation <= myElevation + 1)
            {
                m->y--;
                findRouteBruteforce(m, step + 1);
                m->y++;
            }
        }
    }

    if (m->y < m->height - 1)
    {
        if (!m->data[address + m->width].visited)
        {
            if (m->data[address + m->width].elevation <= myElevation + 1)
            {
                m->y++;
                findRouteBruteforce(m, step + 1);
                m->y--;
            }
        }
    }

    m->data[address].visited = false;
    return;
}

void examineNeighbour(Map* m, int winner, int target)
{
    if (m->data[target].visited) return;

    char myElevation = m->data[winner].elevation;
    int myDistance = m->data[winner].distance;

    if (m->data[target].elevation <= myElevation + 1)
    {
        int newDistance = myDistance + 1;
        if (m->data[target].distance > newDistance)
        {
            m->data[target].distance = newDistance;
            m->data[target].previous = winner;
        }
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
        step++;
        i = m->data[i].previous;
//        printf("%c-", m->data[i].picture);
    }

    printf("\n");

    return step;
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
        .shortestTrip = 0,
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

    findStart(&map);

    //findRouteBruteforce(&map, 0);
    //printf("Shortest walk took %d steps.\n", map.shortestTrip);

    findRouteDijkstra(&map);

    //printDistances(&map);

    printf("Shortest walk took %d steps.\n", countRoute(&map));

    return 0;
}


