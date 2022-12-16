#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

#define DIR_SIZE_LIMIT 100000
#define TOTAL_SPACE 70000000
#define REQUIRED_SPACE 30000000

typedef struct List List;
typedef struct Dir Dir;

struct Dir {
    char* name;
    int totalSize;
    Dir* parent;
    int elementsCount;
    List* elements;
};

struct List {
    bool file;
    char* name;
    int size;
    Dir* dir;
};

Dir* top = NULL;
Dir* current = NULL;
Dir* delete = NULL;
int dirDelta = TOTAL_SPACE;
int smallDirsSize = 0;

Dir* initiateDir(Dir* parent, const char* name)
{
    Dir* d = malloc(sizeof *d);
    assert(d);

    d->parent = parent;
    d->elementsCount = 0;
    d->elements = NULL;
    d->totalSize = 0;
    d->name = strdup(name);

    return d;
}

void populateDir(Dir* d, int size, char* name)
{
    d->elementsCount++;
    d->elements = realloc(d->elements, d->elementsCount * sizeof(List));
    assert(d->elements);

    List* newEl = d->elements + d->elementsCount - 1; // at the end of list
    newEl->name = strdup(name);
    newEl->size = size;
    if (size == 0)
    {
        newEl->file = false;
        newEl->dir = initiateDir(d, name);
    } else
        newEl->file = true;
}

Dir* changeDir(Dir* d, const char* name)
{
    if (!d)
    {
        top = initiateDir(NULL, name);
        return top;
    }

    if (strcmp(name, "..") == 0) return d->parent;

    for (int i = 0; i < d->elementsCount; ++i)
    {
        if (d->elements[i].file) continue;

        if (strcmp(d->elements[i].name, name) == 0) return d->elements[i].dir;
    }

    // not found
    return NULL;
}

void findDirToDelete(Dir* d, int required)
{
    if (d->totalSize > required)
    {
        int delta = d->totalSize - required;
        if (delta < dirDelta)
        {
            delete = d;
            dirDelta = delta;
        }
    }

    for (int i = 0; i < d->elementsCount; ++i)
        if (!d->elements[i].file) findDirToDelete(d->elements[i].dir, required);
}

/*
void printAll(Dir* d, int level)
{
    if (!d) return;
    for (int i = 0; i < level; ++i) printf("   ");
    printf("total size: %d, nr of elements: %d\n", d->totalSize, d->elementsCount);

    for (int i = 0; i < d->elementsCount; ++i)
    {
        for (int i = 0; i < level; ++i) printf("   ");
        printf("%s %d\n", d->elements[i].name, d->elements[i].size);

        if (!d->elements[i].file) printAll(d->elements[i].dir, level + 1);
    }
}
*/

int calculateDirSize(Dir* d)
{
    d->totalSize = 0;     // just in case

    for (int i = 0; i < d->elementsCount; ++i)
    {
        d->totalSize += d->elements[i].size;
        if (!d->elements[i].file) d->totalSize += calculateDirSize(d->elements[i].dir);
    }

    if (d->totalSize <= DIR_SIZE_LIMIT) smallDirsSize += d->totalSize;

    return d->totalSize;
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
    FILE* f = openFile("./input/day7.txt");

    char param1[100] = {0};
    char param2[100] = {0};
    while (fscanf(f, "%s %s", param1, param2) != 0)
    {
        if (feof(f)) break;

        if (strcmp(param1, "$") == 0)
        {
            if (strcmp(param2, "ls") == 0) continue;

            char param3[100]={0};
            fscanf(f, "%s", param3);
            current = changeDir(current, param3);
            if (!current)
            {
                printf("Dir %s not found!\n",param3);
                exit(1);
            }
            continue;
        }

        int size = atoi(param1);
        populateDir(current, size, param2);
    }
    fclose(f);

    calculateDirSize(top);

    //printAll(top, 0);

    printf("Total size of dirs smalled than %d is %d\n", DIR_SIZE_LIMIT, smallDirsSize);
    printf("Minimum to delete: %d\n", REQUIRED_SPACE - (TOTAL_SPACE - top->totalSize));

    findDirToDelete(top, REQUIRED_SPACE - (TOTAL_SPACE - top->totalSize));

    if (delete)
        printf("Best candidate to remove is dir %s, size: %d\n", delete->name, delete->totalSize);
    else
        printf("Not found...");

    return 0;
}
