#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

typedef struct Element {
    char crate;
    struct Element* next;
} Element;

typedef struct {
    int id;
    Element* head;
} Stack;


void pushStack(Stack* s, char crate)
{
    Element* e = malloc(sizeof *e);
    assert(e);

    e->crate = crate;
    e->next = s->head;

    s->head = e;
}

char popStack(Stack* s)
{
    if (!s->head) return 0;

    char ret = s->head->crate;

    s->head = s->head->next;

    return ret;
}

void stackIt(Stack* s, char crate)
{
    Element* e = malloc(sizeof *e);
    assert(e);

    e->crate = crate;
    e->next = NULL;

    if (!s->head)
    {
        s->head = e;
        return;
    }

    Element* p;
    for (p = s->head; p->next != NULL; p = p->next);

    p->next = e;

}

// void printStack(Stack* s)
// {
//     for (Element* e = s->head; e != NULL; e = e->next)
//     {
//         printf("Element %c\n", e->crate);
//     }
// }

FILE* openFile(void)
{
    FILE* f = fopen("./input/day5.txt", "r");
    if (!f)
    {
        fprintf(stderr, "ERROR: can't open input file.\n");
        exit(1);
    }
    return f;
}

int main(void)
{
    FILE* f;

    f = openFile();

    bool initiateStacks = true;
    bool populateStacks = false;
    bool executeMovements = false;

    Stack* stack;
    int stacksNumber = 1;

    char line[100];

    while (fgets(line, sizeof line, f))
    {
        if (*line == '\n')
        {
            initiateStacks = false;
            populateStacks = false;
            executeMovements = true;
            continue;
        }

        if (initiateStacks)
        {
            int len = strlen(line);
            stacksNumber = 1 + (len + 1) / 4;

            stack = malloc(sizeof *stack * stacksNumber);
            assert(stack);

            for (int i = 0; i < stacksNumber; ++i)
            {
                stack[i].head = NULL;
                stack[i].id = i;
            }

            initiateStacks = false;
            populateStacks = true;
        }

        if (populateStacks)
        {
            int offset = 1;
            for (int i = 1; i < stacksNumber; ++i)
            {
                char crate = line[offset];
                if (crate != ' ') stackIt(stack + i, crate);
                offset += 4;
            }
            continue;
        }

        if (executeMovements)
        {
            int quantity;
            int from;
            int to;
            sscanf(line, "move %d from %d to %d", &quantity, &from, &to);
            /*
             * for 1 question 
             *
            for (int i = 0; i < quantity; ++i)
            {
                char crate = popStack(stack + from);
                pushStack(stack + to, crate);
            }
            */

            // this is for part 2:
            for (int i = 0; i < quantity; ++i)
            {
                char crate = popStack(stack + from);
                pushStack(stack, crate);       // stack 0 = buffer
            }
            for (int i = 0; i < quantity; ++i)
            {
                char crate = popStack(stack);  // unload buffer to final destination
                pushStack(stack + to, crate);
            }
            // end part 2
        }


    }
    fclose(f);

//   for (int i = 1; i < stacksNumber; ++i)
//   {
//       printf("Stack %d:\n", i);
//       printStack(stack + i);
//   }

    printf("Answer: ");
    for (int i = 1; i < stacksNumber; ++i)
    {
        char crate = popStack(stack + i);
        printf("%c", crate);
    }
    printf("\n");
    free(stack);

    return 0;
}


