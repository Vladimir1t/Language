#ifndef ASSEMBLER_H_INCLUDED
#define ASSEMBLER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "..\..\Stack\include\stack.h"
#include "..\..\Onegin\include\onegin.h"
#include "..\..\Library\errors.h"
#include "..\..\Library\DSL.h"
#include "..\..\Library\commands.h"

const size_t REGISTRS_NUM =  4;

const size_t ARG_SIZE     =  4;
const size_t COM_SIZE     =  2;

const size_t POISON_ADR   = -1;
const size_t MARK_LEN     =  6;
const char  NUM_MOD       =  0;   /* 0000 0000 */
const char  REG_MOD       =  1;   /* 0000 0001 */
const char  RAM_MOD       =  2;   /* 0000 0011 */

struct mark
{
    char   name[MARK_LEN];
    size_t address;
};

struct registrs
{
    char   name[3];
    elem_t value;
};

struct registrs reg[REGISTRS_NUM] =
{
    {"ax", POISON},  // 23
    {"bx", POISON},  // 24
    {"cx", POISON},  // 25
    {"dx", POISON}   // 26
};

int ReadSourseFile (FILE* sourseF, struct Strings* Str);

int MakeBytecode (FILE* resultF, struct Strings* Str);

int GetArgument (char* command, char* buffer, size_t* ptr, struct mark* allMarks, size_t markNum);

int FindMarks (struct Strings* Str, size_t* len, struct mark* allMarks, size_t* marksNum);

char *strtokR (char *str, const char *delim);

#endif // ASSEMBLER_H_INCLUDED
