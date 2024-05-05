#ifndef DSL_H_INCLUDED
#define DSL_H_INCLUDED

  /* Library of defines */

#define PUSH_COM(arg)            \
    if (arg == NULL)             \
        return NULL_PTR;         \
    StackPush (&proc->stk, arg); \
    IP += ARG_SIZE;


#define POP_COM(arg)                     \
    do                                   \
    {                                    \
        elem_t element = POISON;         \
        StackPop (&proc->stk, &element); \
        *arg = element;                  \
        IP += ARG_SIZE;                  \
    }                                    \
    while (0)


#define checkPtr(ptr)     \
    if (ptr == NULL)      \
        return NULL_PTR;


#define REALLOC(elem, type, size)                            \
    do                                                       \
    {                                                        \
        void* clTmp = realloc (elem, size * sizeof (type));  \
        if (clTmp == NULL)                                   \
            return REALLOC_ERROR;                            \
        elem = (type*) clTmp;                                \
    }                                                        \
    while (0)


#define CALLOC(elem, type, size)                     \
    do                                               \
    {                                                \
        void* clTmp = calloc (size, sizeof (type));  \
        if (clTmp == NULL)                           \
            return CALLOC_ERROR;                     \
        elem = (type*) clTmp;                        \
    }                                                \
    while (0)


#define FOPEN(ptr, name, mode)       \
    FILE* ptr = fopen (name, mode);  \
    if (ptr == NULL)                 \
        return OPEN_ERROR;


#define MATH_COM(command, operand)   \
    GetDigits (proc, &num2, &num1);  \
    num3 = num1 operand num2;        \
    StackPush (&proc->stk, &num3);   \
    num1 = POISON;                   \
    num2 = POISON;


#define MATH_SQRT(command)             \
    StackPop (&proc->stk, &num1);      \
    num1 = (elem_t) sqrt(num1);        \
    StackPush (&proc->stk, &num1);     \
    num1 = POISON;


#define JUMP(command, operand)                        \
    if (proc->stk.data[proc->stk.size - 2] operand    \
        proc->stk.data[proc->stk.size - 1])           \
        IP = JumpTo (proc, IP);                       \
    StackPop (&proc->stk, &num1);                     \
    StackPop (&proc->stk, &num1);                     \
    num1 = POISON;


#define CALL_COM(IP)                         \
    address = IP;                            \
    StackPush (&proc->stkAdr, &address);     \
    IP = JumpTo (proc, IP);                  \
    address = POISON;


#define RET_COM(IP)                          \
    StackPop (&proc->stkAdr, &address);      \
    IP = address;                            \
    address = POISON;


#define OUT_COM()                                              \
    for (int j = 0; j < proc->stk.size; j++)                   \
    {                                                          \
        fprintf (resultF, "[%d] %d\n", j, proc->stk.data[j]);  \
        printf ("[%d] %d\n", j, proc->stk.data[j]);            \
    }                                                          \
    StackPop (&proc->stk, &num1);



#define IN_COM()                           \
    printf ("write a number\n");           \
    scanf (SPEC, &num1);                   \
    StackPush (&proc->stk, &num1);         \
    num1 = POISON;


#define HLT_COM()        \
    fclose (resultF);    \
    CpuDtor (proc);      \
    return SUCCESS;

#endif // DSL_H_INCLUDED
