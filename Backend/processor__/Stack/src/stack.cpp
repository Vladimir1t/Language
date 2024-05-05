#include "..\include\stack.h"

FILE* ERROR_FILE = fopen ("Log\\error_file_stack.txt", "w");

void StackCtor (struct Stack* stk, const size_t capacity)
{
    if (capacity <= 0)
        fprintf ( ERROR_FILE, "Error. Capacity <= 0\n");
    stk->size = 0;
    stk->capacity = capacity;
    stk->data = (elem_t*) calloc (stk->capacity, sizeof(elem_t));

    if (stk->data == NULL)
        fprintf (ERROR_FILE, "ERROR. Calloc");

    for (int i = 0; i < capacity; i++)
        stk->data[i] = POISON;

    Verifier (stk);
}

void StackPush (struct Stack* stk, const elem_t* value)
{
    Verifier (stk);

    if (stk->size == stk->capacity)
        StackReallocIncrease (stk);

    stk->data[stk->size] = *value;

    stk->size += 1;

    //printf ("size stk %d\n", stk->size);
    Verifier (stk);
}

void StackPop (struct Stack* stk, elem_t* element)
{
    Verifier (stk);

    assert (element != NULL);

    if (stk->size == 0)
    {
        printf  ("there are no numbers to be popped\n");
        fprintf (ERROR_FILE, "<< there are no numbers to be popped >>\n");
    }

    if (stk->size * 4 < stk->capacity)
        StackReallocDecrease (stk);

    *element = stk->data[stk->size - 1];
    stk->data[stk->size - 1] = POISON;

    stk->size -= 1;

    //printf ("size stk %d\n", stk->size);
    Verifier (stk);
}

void StackDtor (struct Stack* stk)
{
    Verifier (stk);

    for (size_t i = 0; i < stk->size; i++)
        stk->data[i] = POISON;

    stk->capacity = 0;
    stk->size = 0;

    free (stk->data);

    stk->data = NULL;
}

int StackReallocIncrease (struct Stack* stk)
{
     Verifier (stk);

     elem_t* pointer = stk->data;
     stk->data = (elem_t*) realloc (stk->data, (stk->capacity * 2) * sizeof(elem_t));

     if (stk->data == NULL)
     {
        stk->data = pointer;
        fprintf (ERROR_FILE, "<< Realloc increase error >>\n");
        return REALLOC_ERR;
     }
     for (int i = stk->capacity; i < stk->capacity * 2; i++)
        stk->data[i] = POISON;

     stk->capacity *= 2;

     Verifier (stk);

     return REALLOC_OK;
}

int StackReallocDecrease (struct Stack* stk)
{
     Verifier (stk);

     elem_t* pointer = stk->data;
     stk->data = (elem_t*) realloc (stk->data, (stk->capacity / 2) * sizeof(elem_t));
     if (stk->data == NULL)
     {
        stk->data = pointer;
        fprintf (ERROR_FILE, "<< Realloc decrease error >>\n");
        return REALLOC_ERR;
     }
     stk->capacity /= 2;

     Verifier (stk);

     return REALLOC_OK;
}

void Dump (struct Stack* stk, FILE* logFile)
{
    Verifier (stk);
    assert (logFile != NULL);

    fprintf (logFile, "------------------\n");
    fprintf (logFile, "----Stack dump----\n");
    fprintf (logFile, "------------------\n");

    fprintf (logFile, "capacity: %d\n", stk->capacity);
    fprintf (logFile, "size: %d\n", stk->size);

    for (int i = 0; i < stk->size; i++)
        fprintf (logFile, "[%d] " SPEC "\n", i, stk->data[i]);
    fprintf (logFile, "------------------\n");

    for (int i = stk->size; i < stk->capacity; i++)
        fprintf (logFile, "[%d] " SPEC "\n", i, stk->data[i]);
    fprintf (logFile, "------------------\n");

    Verifier (stk);
}

void Verifier (struct Stack* stk)
{
    if (stk == NULL)
        fprintf (ERROR_FILE, "Error: Stack = NULL\n");

    if (stk->data == NULL)
        fprintf (ERROR_FILE, "Error: stk.data = NULL\n");

    if (stk->capacity < stk->size)
        fprintf (ERROR_FILE, "Error: capacity < size\n");

    if (stk->capacity < 0)
        fprintf (ERROR_FILE, "ERROR: capacity < 0\n");

    if (stk->size < 0)
        fprintf (ERROR_FILE, "ERROR: size < 0\n");

    for (int i = 0; i < stk->size; i++)
    {
        if (stk->data[i] == POISON)
            fprintf (ERROR_FILE, "element [%d] = POISON value\n", i);
    }
    for (int i = stk->size; i < stk->capacity; i++)
    {
        if (stk->data[i] != POISON)
            fprintf (ERROR_FILE, "element [%d] != POISON value\n", i);
    }
}
