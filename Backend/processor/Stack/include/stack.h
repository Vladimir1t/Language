#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SPEC "%d"

typedef int elem_t;
const elem_t POISON = 0xDEAD;

struct Stack
{
    size_t  capacity;
    size_t  size;
    elem_t* data;
};

enum ReallocMod
{
    REALLOC_OK  = 1,
    REALLOC_ERR = 0
};

enum Errors
{
    CAPACITY_BELLOW_ZERO = 0,
    SIZE_BELLOW_ZERO = 1,
};

void StackCtor (Stack* stk, const size_t capacity);

void StackPush (Stack* stk, const elem_t* value);

void StackPop  (Stack* stk, elem_t* element);

void StackDtor (Stack* stk);

int StackReallocIncrease (Stack* stk);

int StackReallocDecrease (Stack* stk);

void Verifier (Stack* stk);

void Dump (Stack* stk, FILE* file);

#endif // STACK_H_INCLUDED
