#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_TUNELS 5

#define INPUT_FILE "input/day16.txt"

/* 
 * I'm not sure I can further optimize this program without total rethink of algorithm
 * It's still slow. Part 1 is solved in under 1 minute on my thin Linux desktop
 * but part two takes painfully long time.
 *
 * to reduce impact on memory, change #define PART 2:
 */

#define PART 12

#if PART == 2
#define TIME 26
#else
#define TIME 30
#endif

typedef struct valve
{
    char name[3];
    int flow;
    char tunnel[MAX_TUNELS][3];
    struct valve* next_v[MAX_TUNELS];
    int t_num;
    int open;
    unsigned char valveID;
} Valve;

typedef struct cache
{
    Valve* valve2;
    unsigned int score;
    uint64_t valves_status_mask;
    struct cache* next;
} Cache;

typedef struct
{
    Valve* valves;
    int count;
    Cache* (*cache_head)[TIME];   // weird notation, to have array[][] of pointers to Cache*
} ValvesSet;

Valve* find_valve(ValvesSet* all, char* name)
{
    for (int i = 0; i < all->count ; ++i)
        if (strcmp(all->valves[i].name, name) == 0) return all->valves + i;

    return NULL;
}

// used in part 1
int visit_valve(Valve* v, ValvesSet* all, int const time)
{
    if (time == 0) return 0;

    Cache* cache = all->cache_head[v->valveID][time - 1];
    if (cache)
    {
        // calculate mask from currently open valves
        uint64_t valves_status = 0;
        for (int i = 0; i < all->count; ++i)
            valves_status |= ((uint64_t)all->valves[i].open << i);
        if (valves_status == cache->valves_status_mask) return cache->score;
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

    // create cache entry
    all->cache_head[v->valveID][time - 1] = (Cache*) malloc(sizeof(Cache));
    if (! all->cache_head[v->valveID][time - 1])
       {
           fprintf(stderr, "Can't allocate memory");
           exit(1);
       }

    all->cache_head[v->valveID][time - 1]->score = max;
    all->cache_head[v->valveID][time - 1]->next = NULL;
    all->cache_head[v->valveID][time - 1]->valves_status_mask = 0;
    for (int i = 0; i < all->count; ++i)
        if (all->valves[i].open)
                all->cache_head[v->valveID][time - 1]->valves_status_mask |= ((uint64_t)1 << i);

    return max;
}

// used in part 2
int visit_2_valves(Valve* v1, Valve* v2, ValvesSet* all, int const time)
{
    if (time == 0) return 0;

    for (Cache* cache = all->cache_head[v1->valveID][time - 1]; cache != NULL; cache =cache->next)
        if (cache->valve2 == v2)
        {
            uint64_t valves_status = 0;
            for (int i = 0; i < all->count; ++i)
                valves_status |= ((uint64_t)all->valves[i].open << i);

            if (valves_status == cache->valves_status_mask) return cache->score;
        }

    int max = 0;
    // #1 we both open valves
    if (v1->open == 0 && v1->flow != 0 &&
        v2->open == 0 && v2->flow != 0 &&
        v1 != v2)   // can't both open the same valve!
    {
        v1->open = 1;
        v2->open = 1;
        // flow will be counting from next minute, hence -1
        max = v1->flow * (time - 1);
        max += v2->flow * (time - 1);
        // visit the same valves, now they will be opened
        max += visit_2_valves(v1, v2, all, time - 1);
        v1->open = 0;  // close it back
        v2->open = 0;  // close it back
        // and continue tests of other scenarios
    }
    
    // #2 I open valve, and elephant moves forward
    if (v1->open == 0 && v1->flow != 0)
    {
        v1->open = 1;
        int temp_max2 = v1->flow * (time - 1);
        int tunel_visit_max = 0;
        // my valve is open, and elephant goes to his tunnels
        for (int i = 0; i < v2->t_num; ++i)
        {
            int temp_max = visit_2_valves(v1, v2->next_v[i], all, time - 1);
            if (tunel_visit_max < temp_max) tunel_visit_max = temp_max;
        }
        v1->open = 0;
        // update max, if it was better, than previous scenario
        if (max < temp_max2 + tunel_visit_max) max = temp_max2 + tunel_visit_max;
    }
    // #3 Elephant opens valve, and I move forward
    if (v2->open == 0 && v2->flow != 0)
    {
        v2->open = 1;
        int temp_max2 = v2->flow * (time - 1);
        int tunel_visit_max = 0;
        // same as above, just change of roles
        for (int i = 0; i < v1->t_num; ++i)
        {
            int temp_max = visit_2_valves(v1->next_v[i], v2, all, time - 1);
            if (tunel_visit_max < temp_max) tunel_visit_max = temp_max;
        }
        v2->open = 0;
        if (max < temp_max2 + tunel_visit_max) max = temp_max2 + tunel_visit_max;
    }

    // #4 we don't open anything, just visit linked tunnels 
    for (int i = 0; i < v1->t_num; ++i)
        for (int j = 0; j < v2->t_num; ++j)
        {
            int temp_max = visit_2_valves(v1->next_v[i], v2->next_v[j], all, time - 1);
            if (max < temp_max) max = temp_max;
        }

    Cache* new_cache = (Cache*) malloc(sizeof(Cache));
    if (! new_cache)
       {
           fprintf(stderr, "Can't allocate memory");
           exit(1);
       }

    // save results to cache
    new_cache->valve2 = v2;
    new_cache->score = max;
    // set bits in mask for all open valves
    new_cache->valves_status_mask = 0;
    for (int i = 0; i < all->count; ++i)
         new_cache->valves_status_mask |= ((uint64_t)all->valves[i].open << i);

    new_cache->next = all->cache_head[v1->valveID][time - 1];
    all->cache_head[v1->valveID][time - 1] = new_cache;

    return max;
}

void free_cache(ValvesSet* all)
{
    //[TODO] for sure there is a smarter way to deallocate cache memory
    for (int i = 0; i < TIME; ++i)
        for (int j = 0; j < all->count; ++j)
            while (all->cache_head[j][i] != NULL)
            {
                Cache* tmp = all->cache_head[j][i];
                all->cache_head[j][i] = all->cache_head[j][i]->next;

                free(tmp);
            }
}

int main(void)
{
    ValvesSet all;
    all.valves = NULL;
    all.count = 0;

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

    for (all.count = 0; getline(&line, &len, input) != -1; ++all.count)
    {
        all.valves = (Valve*) realloc(all.valves, (all.count + 1) *  sizeof(Valve));
        if (! all.valves)
        {
            fprintf(stderr, "Can't allocate memory.");
            exit_status = EXIT_FAILURE;
            goto cleanup;
        }

        sscanf(line, "%*s %s", all.valves[all.count].name);
        all.valves[all.count].flow = atoi(line + 23);
        all.valves[all.count].open = 0;
        all.valves[all.count].t_num = 0;
        all.valves[all.count].valveID = all.count;       // for better cache

        int i = 0;
        char* tunnel = strtok(line + 25, " ,;\n");
        do
            if (isupper(*tunnel))
            {
                strcpy(all.valves[all.count].tunnel[i++], tunnel);
                all.valves[all.count].t_num++;
            }
        while ((tunnel = strtok(NULL, " ,;\n")) != NULL);
    }

    all.cache_head = malloc(all.count * sizeof(Cache*[TIME]));
    if (! all.cache_head)
    {
        fprintf(stderr, "Can't allocate memory.");
        exit_status = EXIT_FAILURE;
        goto cleanup;
    }

    // create pointer indexes for faster traversing
    for (int i = 0; i < all.count; ++i)
        for (int j = 0; j < all.valves[i].t_num; ++j)
            all.valves[i].next_v[j] = find_valve(&all, all.valves[i].tunnel[j]);

    Valve* start = find_valve(&all, "AA");

    int pressure; 
#if PART == 12
    pressure = visit_valve(start, &all, TIME);
    printf("Released pressure: %d\n", pressure);

    free_cache(&all);
#endif
    pressure = visit_2_valves(start, start, &all, 26);
    printf("Released pressure in part 2: %d\n", pressure);

cleanup:
    if (input) fclose(input);
    free_cache(&all);
    free(all.valves);
    exit (exit_status);
}

