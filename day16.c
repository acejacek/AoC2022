#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>

#define MAX_TUNELS 5

#define INPUT_FILE "input/day16.txt"

typedef struct valve
{
    char name[3];
    int flow;
    char tunnel[MAX_TUNELS][3];
    struct valve* next_v[MAX_TUNELS];
    int t_num;
    int open;
} Valve;

typedef struct cache
{
    Valve* valve;
    Valve* valve2;
    int score;
    int time;
    int valves_status[100];
    struct cache* next;
} Cache;

typedef struct
{
    Valve* valves;
    int count;
} ValvesSet;

Cache* cache_head = NULL;

Valve* find_valve(ValvesSet* all, char* name)
{
    for (int i = 0; i < all->count ; ++i)
        if (strcmp(all->valves[i].name, name) == 0) return all->valves + i;

    return NULL;
}

int visit_valve(Valve* v, ValvesSet* all, int const time)
{
    if (time == 0) return 0;

    for (Cache* cache = cache_head; cache != NULL; cache = cache->next)
        if (cache->time == time && cache->valve == v)
        {
            int same = 1;
            for (int i = 0; i < all->count; ++i)
                if (all->valves[i].open != cache->valves_status[i])
                {
                    same = 0;
                    break;
                }
            if (same) return cache->score;
        }

    int max = 0;
    // this valve can be opened & it makes sense to open it (flow > 0)
    if (v->open == 0 && v->flow != 0)
    {
        v->open = 1;
        // flow will be counting from next minute, hence -1
        max = v->flow * (time - 1);
        // visit the same valve, now it will be opened
        max += visit_valve(v, all, time - 1);
        v->open = 0;  // close it back
        // and continue test, if score will be better when valve remains closed
    }
    // visit linked tunnels 
    for (int i = 0; i < v->t_num; ++i)
    {
        int temp_max = visit_valve(v->next_v[i], all, time - 1);
        if (max < temp_max) max = temp_max;
    }

    Cache* new_cache = (Cache*) malloc(sizeof(Cache));
    if (new_cache == NULL)
       {
           fprintf(stderr, "Can't allocate memory");
           exit(1);
       }

    new_cache->valve = v;
    new_cache->time = time;
    new_cache->score = max;
    for (int i = 0; i < all->count; ++i)
        new_cache->valves_status[i] = all->valves[i].open;

    new_cache->next = cache_head;
    cache_head = new_cache;

    return max;
}

int visit_2_valves(Valve* v1, Valve* v2, ValvesSet* all, int const time)
{
    if (time == 0) return 0;

    for (Cache* cache = cache_head; cache != NULL; cache = cache->next)
        if (
                cache->time == time &&
                cache->valve == v1 &&
                cache->valve2 == v2
           )
        {
            int same = 1;
            for (int i = 0; i < all->count; ++i)
                if (all->valves[i].open != cache->valves_status[i])
                {
                    same = 0;
                    break;
                }
            if (same) return cache->score;
        }

    int max = 0;
    // #1 both open valves
    if (v1->open == 0 && v1->flow != 0 &&
        v2->open == 0 && v2->flow != 0 &&
        v1 != v2)   // can't both open the same valve!
    {
        v1->open = 1;
        v2->open = 1;
        // flow will be counting from next minute, hence -1
        max = v1->flow * (time - 1);
        max += v2->flow * (time - 1);
        // visit the same valve, now it will be opened
        max += visit_2_valves(v1, v2, all, time - 1);
        v1->open = 0;  // close it back
        v2->open = 0;  // close it back
        // and continue test, if score will be better when valve remains closed
    }
    
    // #2 I open valve and elephant moves forward
    if (v1->open == 0 && v1->flow != 0)
    {
        v1->open = 1;
        int temp_max2 = v1->flow * (time - 1);
        int tunel_visit_max = 0;
        for (int i = 0; i < v2->t_num; ++i)
        {
            int temp_max = visit_2_valves(v1, v2->next_v[i], all, time - 1);
            if (tunel_visit_max < temp_max) tunel_visit_max = temp_max;
        }
        v1->open = 0;
        if (max < temp_max2 + tunel_visit_max) max = temp_max2 + tunel_visit_max;
    }
    // #3 Elephant opens valve and I move forward
    if (v2->open == 0 && v2->flow != 0)
    {
        v2->open = 1;
        int temp_max2 = v2->flow * (time - 1);
        int tunel_visit_max = 0;
        for (int i = 0; i < v1->t_num; ++i)
        {
            int temp_max = visit_2_valves(v1->next_v[i], v2, all, time - 1);
            if (tunel_visit_max < temp_max) tunel_visit_max = temp_max;
        }
        v2->open = 0;
        if (max < temp_max2 + tunel_visit_max) max = temp_max2 + tunel_visit_max;
    }

    // #4 just visit linked tunnels 
    for (int i = 0; i < v1->t_num; ++i)
        for (int j = 0; j < v2->t_num; ++j)
        {
            int temp_max = visit_2_valves(v1->next_v[i], v2->next_v[j], all, time - 1);
            if (max < temp_max) max = temp_max;
        }

    Cache* new_cache = (Cache*) malloc(sizeof(Cache));
    if (new_cache == NULL)
       {
           fprintf(stderr, "Can't allocate memory");
           exit(1);
       }

    new_cache->time = time;
    new_cache->valve = v1;
    new_cache->valve2 = v2;
    new_cache->score = max;
    for (int i = 0; i < all->count; ++i)
        new_cache->valves_status[i] = all->valves[i].open;

    new_cache->next = cache_head;
    cache_head = new_cache;

    return max;
}

void free_cache(void)
{
    while (cache_head != NULL)
    {
        Cache* tmp = cache_head;
        cache_head = cache_head->next;
        free(tmp);
    }
}

int main(void)
{
    Valve* valves = NULL;
    int v;

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

    for (v = 0; getline(&line, &len, input) != -1; ++v)
    {
        valves = (Valve*) realloc(valves, (v + 1) *  sizeof(Valve));
        if (! valves)
        {
            fprintf(stderr, "Can't allocate memory.");
            exit_status = EXIT_FAILURE;
            goto cleanup;
        }

        sscanf(line, "%*s %s", valves[v].name);
        valves[v].flow = atoi(line + 23);
        valves[v].open = 0;
        valves[v].t_num = 0;

        int i = 0;
        char* tunnel = strtok(line + 25, " ,;\n");
        do
            if (isupper(*tunnel))
            {
                strcpy(valves[v].tunnel[i++], tunnel);
                valves[v].t_num++;
            }
        while ((tunnel = strtok(NULL, " ,;\n")) != NULL);
    }

    ValvesSet all = { valves, v };

    // create pointer indexes for faster traversing
    for (int i = 0; i < v; ++i)
        for (int j = 0; j < valves[i].t_num; ++j)
            valves[i].next_v[j] = find_valve(&all, valves[i].tunnel[j]);

    Valve* start = find_valve(&all, "AA");

    int pressure; 
//    pressure = visit_valve(start, &all, 30);
//    printf("Released pressure: %d\n", pressure);

    free_cache();

    pressure = visit_2_valves(start, start, &all, 26);
    printf("Released pressure in part 2: %d\n", pressure);

cleanup:
    if (input) fclose(input);
    free_cache();
    free(valves);
    exit (exit_status);
}

