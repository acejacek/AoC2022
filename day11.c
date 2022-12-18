#include <stdio.h>
#include <stdlib.h>

// change to 1 for solution for part 1:
#define PART 2

#define NUMBER_OF_MONKEYS 8

typedef struct Monkey
{
    size_t items[100];
    int itemsCount;
    int operation;
    char testOperator;
    int testValue;
    int throwWhenTrue;
    int throwWhenFalse;
    int inspections;
} Monkey;

Monkey monkeys[8];

void setStaringTestParams(void)
{
    monkeys[0] = (Monkey) {
        .items           = { 79, 98 },
        .itemsCount      = 2,
        .operation       = 19,
        .testOperator    = '*',
        .testValue       = 23,
        .throwWhenTrue   = 2,
        .throwWhenFalse  = 3,
        .inspections     = 0,
    };
    monkeys[1] = (Monkey) {
        .items           = { 54, 65, 75, 74},
        .itemsCount      = 4,
        .operation       = 6,
        .testOperator    = '+',
        .testValue       = 19,
        .throwWhenTrue   = 2,
        .throwWhenFalse  = 0,
        .inspections     = 0,
    };
    monkeys[2] = (Monkey) {
        .items           = { 79, 60, 97 },
        .itemsCount      = 3,
        .operation       = 2,
        .testOperator    = '^',
        .testValue       = 13,
        .throwWhenTrue   = 1,
        .throwWhenFalse  = 3,
        .inspections     = 0,
    };
    monkeys[3] = (Monkey) {
        .items           = { 74 },
        .itemsCount      = 1,
        .operation       = 3,
        .testOperator    = '+',
        .testValue       = 17,
        .throwWhenTrue   = 0,
        .throwWhenFalse  = 1,
        .inspections     = 0,
    };
}

void setStaringParams(void)
{
    monkeys[0] = (Monkey) {
        .items           = { 66, 79 },
        .itemsCount      = 2,
        .operation       = 11,
        .testOperator    = '*',
        .testValue       = 7,
        .throwWhenTrue   = 6,
        .throwWhenFalse  = 7,
        .inspections     = 0,
    };
    monkeys[1] = (Monkey) {
        .items           = { 84, 94, 94, 81, 98, 75 },
        .itemsCount      = 6,
        .operation       = 17,
        .testOperator    = '*',
        .testValue       = 13,
        .throwWhenTrue   = 5,
        .throwWhenFalse  = 2,
        .inspections     = 0,
    };
    monkeys[2] = (Monkey) {
        .items           = { 85, 79, 59, 64, 79, 95, 67 },
        .itemsCount      = 7,
        .operation       = 8,
        .testOperator    = '+',
        .testValue       = 5,
        .throwWhenTrue   = 4,
        .throwWhenFalse  = 5,
        .inspections     = 0,
    };
    monkeys[3] = (Monkey) {
        .items           = { 70 },
        .itemsCount      = 1,
        .operation       = 3,
        .testOperator    = '+',
        .testValue       = 19,
        .throwWhenTrue   = 6,
        .throwWhenFalse  = 0,
        .inspections     = 0,
    };
    monkeys[4] = (Monkey) {
        .items           = { 57, 69, 78, 78 },
        .itemsCount      = 4,
        .operation       = 4,
        .testOperator    = '+',
        .testValue       = 2,
        .throwWhenTrue   = 0,
        .throwWhenFalse  = 3,
        .inspections     = 0,
    };
    monkeys[5] = (Monkey) {
        .items           = { 65, 92, 60, 74, 72 },
        .itemsCount      = 5,
        .operation       = 7,
        .testOperator    = '+',
        .testValue       = 11,
        .throwWhenTrue   = 3,
        .throwWhenFalse  = 4,
        .inspections     = 0,
    };
    monkeys[6] = (Monkey) {
        .items           = { 77, 91, 91 },
        .itemsCount      = 3,
        .operation       = 2,
        .testOperator    = '^',
        .testValue       = 17,
        .throwWhenTrue   = 1,
        .throwWhenFalse  = 7,
        .inspections     = 0,
    };
    monkeys[7] = (Monkey) {
        .items           = { 76, 58, 57, 55, 67, 77, 54, 99 },
        .itemsCount      = 8,
        .operation       = 6,
        .testOperator    = '+',
        .testValue       = 3,
        .throwWhenTrue   = 2,
        .throwWhenFalse  = 1,
        .inspections     = 0,
    };
}

void printMonkeysInventory()
{
    for (int i = 0; i < NUMBER_OF_MONKEYS; ++i)
    {
        printf("Monkey %d: ", i);

        for (int j = 0; j < monkeys[i].itemsCount; ++j)
        {
            printf("%zu ", monkeys[i].items[j]);
        }
        printf("\n   Monkey inspected %d items\n\n", monkeys[i].inspections);
    }
}

void throwItem(int item, int monkeyID)
{
    Monkey* m = monkeys + monkeyID;

    m->items[m->itemsCount] = item;
    m->itemsCount++;
}

void doRound(int monkeyID, int relief)
{
    Monkey* m = monkeys + monkeyID;

    for (int i = 0; i < m->itemsCount; ++i)
    {
        switch (m->testOperator)
        {
            case '+':
                m->items[i] += m->operation;
                break;

            case '*':
                m->items[i] *= m->operation;
                break;

            case '^':
                m->items[i] *= m->items[i];
                break;

            default:
                fprintf(stderr, "Err...\n");
                exit(1);
        }

#if PART == 1
        (void) relief;
        m->items[i] /= 3;
#else
        m->items[i] %= relief;
#endif

        if (m->items[i] % m->testValue == 0)
            throwItem(m->items[i], m->throwWhenTrue);
        else
            throwItem(m->items[i], m->throwWhenFalse);

        m->inspections++;
    }

    m->itemsCount = 0;  // all items thrown away

}

size_t findBestTwo(void)
{
    int best[2] = { 0 };

    for (int i = 0; i < NUMBER_OF_MONKEYS; ++i)
    {
        if (best[0] < monkeys[i].inspections)
        {
            best[1] = best[0];
            best[0] = monkeys[i].inspections;
        }
        else if (best[1] < monkeys[i].inspections)
        {
            best[1] = monkeys[i].inspections;
        }
    }

    return (size_t) best[0] * best[1];
}

int calculateDivider(void)
{
    int result = monkeys[0].testValue;

    for (int i = 1; i < NUMBER_OF_MONKEYS; ++i)
        result *= monkeys[i].testValue;

    return result;
}

int main(void)
{
    setStaringParams();
//    setStaringTestParams();   // set NUMBER_OF_MONKEYS to 4

#if PART == 1
    const int roundLimit = 20;
    const int relief;
#else
    const int roundLimit = 10000;
    const int relief = calculateDivider();
#endif

    int round = 0;
    while (round++ < roundLimit)
    {
        for (int i = 0; i < NUMBER_OF_MONKEYS; ++i)
        {
            doRound(i, relief);
        }
    }

//    printMonkeysInventory();

    printf("Monkey business factor is: %lu\n", findBestTwo());

    return 0;
}

