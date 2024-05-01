#ifndef stack_H_INCLUDED
#define stack_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
//#include "..\..\include\differentiator.h"  //

#define SPEC_STK "%s"

typedef void* elem_t_stk;

const elem_t_stk POISON_STK = NULL;

struct stack
{
    size_t      capacity;
    size_t      size;
    elem_t_stk* data;
};

enum ReallocMod
{
    REALLOC_OK  = 1,
    REALLOC_ERR = 0,
};

enum Errors
{
    CAPACITY_BELLOW_ZERO = 0,
    SIZE_BELLOW_ZERO = 1,
};

void stack_ctor (stack* stk, const size_t capacity);

void stack_push (stack* stk, const elem_t_stk* value);

void stack_pop  (stack* stk, elem_t_stk* element);

void stack_dtor (stack* stk);

int stack_realloc_increase (stack* stk);

int stack_realloc_decrease (stack* stk);

void verifier (stack* stk);

void dump (stack* stk, FILE* file);

#endif // stack_H_INCLUDED
