#ifndef ONEGIN_H_INCLUDED
#define ONEGIN_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

struct String
{
    char* pointer;
    int   size;
};

struct Strings
{
    String* stringsP;
    char*   textPointer;
    int     fileSize;
    int     nStrings;
};

enum Sort
{
    BEGINNING = 1,
    ENDING    = 0,
};

enum fileFormat
{
    YES       =  1,
    NO        =  0,
    UNDEFINED = -1,
};

void ClearMemory (struct Strings* StrP);

fileFormat StringsCount (struct Strings* StrP);

void StringsPointerRead (struct Strings* StrP, fileFormat isR);

int FileSize (FILE* fPointer);

void SortedStrPrint (struct Strings* StrP, int const mod);

void PrintSymbols (struct Strings* StrP);

void Putchar (char a);

void StrPrint (struct Strings* StrP);

#endif // ONEGIN_H_INCLUDED
