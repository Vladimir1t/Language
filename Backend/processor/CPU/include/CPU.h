#ifndef SPU_H_INCLUDED
#define SPU_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "..\..\Assembler\include\assembler.h"
#include "..\..\Stack\include\stack.h"
#include "..\..\Onegin\include\onegin.h"
#include "..\..\Library\commands.h"

//const char*  fileSourse  = "Assembler\\res_ass.txt";  /* bytecode */
const char   MASK        = 3;                         /* 0000 0011 */
const size_t RAM_SIZE    = 50;                        /* size of a RAM */

struct Cpu
{
    Stack   stk;           /* stack for arguments */
    Stack   stkAdr;        /* stack for addresses */
    char*   arrayCommand;  /* buffer of commands and arguments */
    size_t  arraySize;     /* size of the buffer */
    elem_t* ram;           /* random access memory */
    elem_t  ramSize;
};

int ExecuteCommands (FILE* resultF, struct Cpu* proc);

void CpuCtor (struct Cpu* proc);

void CpuDtor (struct Cpu* proc);

int FillArray (struct Cpu* proc);

void GetDigits (struct Cpu* proc, elem_t* num2, elem_t* num1);

size_t JumpTo (struct Cpu* proc, size_t IP);

char GetCommand (struct Cpu* proc, size_t IP);

elem_t* GetArgument (struct Cpu* proc, size_t IP, char mode);

void Push (struct Cpu* proc, size_t IP);

void Pop (struct Cpu* proc, size_t* IP);

#endif // SPU_H_INCLUDED
