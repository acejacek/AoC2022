#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct {
    int from;
    int to;
} Section;

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

int isOverlaping(Section a, Section b)
{
    if (b.from <= a.to && b.to >= a.from) return 1;

    return 0;
}

int isContained(Section a, Section b)
{
    if (a.from <= b.from && b.to <= a.to) return 1;
    if (b.from <= a.from && a.to <= b.to) return 1;

    return 0;
}

int main(void)
{
    FILE* f = openFile("./input/day4.txt");

    Section elf1;
    Section elf2;

    int fullyContained = 0;
    int partialyOverlaps = 0;

    while (fscanf(f, "%d-%d,%d-%d", &elf1.from, &elf1.to, &elf2.from, &elf2.to) != EOF)
    {
        fullyContained += isContained(elf1, elf2);
        partialyOverlaps += isOverlaping(elf1, elf2);
    }
    fclose(f);

    printf("Fully contained ranges: %d\n", fullyContained);
    printf("Partially overlaped ranges: %d\n", partialyOverlaps);

    return 0;
}
