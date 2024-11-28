#include "./../include/stack.h"

FILE* ERROR_FILE = fopen ("Log/file_error_stack.txt", "w");

void stack_ctor (struct stack* stk, const size_t capacity)
{
    if (capacity <= 0)
        fprintf ( ERROR_FILE, "Error. Capacity <= 0\n");
    stk->size = 0;
    stk->capacity = capacity;
    stk->data = (elem_t_stk*) calloc (stk->capacity, sizeof(elem_t_stk));

    if (stk->data == NULL)
        fprintf (ERROR_FILE, "ERROR. Calloc");

    for (int i = 0; i < capacity; i++)
        stk->data[i] = POISON_STK;

    verifier (stk);
}

void stack_push (struct stack* stk, const elem_t_stk* value)
{
    verifier (stk);

    if (stk->size == stk->capacity)
        stack_realloc_increase (stk);

    stk->data[stk->size] = *value;

    stk->size++;

    verifier (stk);
}

void stack_pop (struct stack* stk, elem_t_stk* element)
{
    verifier (stk);

    if (element == NULL)
        fprintf (ERROR_FILE, "<< NULL PTR >>\n");

    if (stk->size == 0)
        fprintf (ERROR_FILE, "<< there are no numbers to be popped >>\n");

    if (stk->size * 4 < stk->capacity)
        stack_realloc_decrease (stk);

    *element = stk->data[stk->size - 1];
    stk->data[stk->size - 1] = POISON_STK;

    stk->size--;

    verifier (stk);
}

void stack_dtor (struct stack* stk)
{
    verifier (stk);

    for (size_t i = 0; i < stk->size; i++)
        stk->data[i] = POISON_STK;

    stk->capacity = 0;
    stk->size = 0;

    free (stk->data);

    stk->data = NULL;
}

int stack_realloc_increase (struct stack* stk)
{
     verifier (stk);

     elem_t_stk* pointer = stk->data;
     stk->data = (elem_t_stk*) realloc (stk->data, (stk->capacity * 2) * sizeof (elem_t_stk));

     if (stk->data == NULL)
     {
        stk->data = pointer;
        fprintf (ERROR_FILE, "<< Realloc increase error >>\n");
        return REALLOC_ERR;
     }
     for (int i = stk->capacity; i < stk->capacity * 2; i++)
        stk->data[i] = POISON_STK;

     stk->capacity *= 2;

     verifier (stk);

     return REALLOC_OK;
}

int stack_realloc_decrease (struct stack* stk)
{
     verifier (stk);

     elem_t_stk* pointer = stk->data;
     stk->data = (elem_t_stk*) realloc (stk->data, (stk->capacity / 2) * sizeof (elem_t_stk));
     if (stk->data == NULL)
     {
        stk->data = pointer;
        fprintf (ERROR_FILE, "<< Realloc decrease error >>\n");
        return REALLOC_ERR;
     }
     stk->capacity /= 2;

     verifier (stk);

     return REALLOC_OK;
}

void dump (struct stack* stk, FILE* logFile)
{
    verifier (stk);
    if (logFile == NULL)
        fprintf (ERROR_FILE, "<< NUL ptr >>\n");

    fprintf (logFile, "--------------------\n");
    fprintf (logFile, "---- stack dump ----\n");
    fprintf (logFile, "--------------------\n");

    fprintf (logFile, "capacity: %d\n", stk->capacity);
    fprintf (logFile, "size: %d\n", stk->size);

    for (int i = 0; i < stk->size; i++)
        fprintf (logFile, "[%d] " SPEC_STK "\n", i, stk->data[i]);
    fprintf (logFile, "--------------------\n");

    for (int i = stk->size; i < stk->capacity; i++)
        fprintf (logFile, "[%d] " SPEC_STK "\n", i, stk->data[i]);
    fprintf (logFile, "--------------------\n");

    verifier (stk);
}

void verifier (struct stack* stk)
{
    if (stk == NULL)
        fprintf (ERROR_FILE, "Error: stack = NULL\n");

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
        if (stk->data[i] == POISON_STK)
            fprintf (ERROR_FILE, "element [%d] = POISON_STK value\n", i);
    }
    for (int i = stk->size; i < stk->capacity; i++)
    {
        if (stk->data[i] != POISON_STK)
            fprintf (ERROR_FILE, "element [%d] != POISON_STK value\n", i);
    }
}

