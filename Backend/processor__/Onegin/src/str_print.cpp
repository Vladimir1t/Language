#include "..\include\Onegin.h"

void StrPrint (struct Strings* StrP)
{
    for (size_t n = 0; n < StrP->nStrings; n++)
        printf ("%d) %s\n", n + 1, StrP->stringsP[n].pointer);
}

void SortedStrPrint (struct Strings* StrP, int const mod, char* fileName)
{
    FILE* filePointerNew = fopen (fileName, "w");               // get a name of file from command line
    if (mod == BEGINNING)
        printf ("-Strings are sorted by the alphabetic order-\n");
    else if (mod == ENDING)
        printf ("-Strings are sorted by the alphabetic order of the end of the lines-\n");
    else
        printf ("Error.");

    for (int n = 0; n < StrP->nStrings; n++)
    {
        fputs (StrP->stringsP[n].pointer, filePointerNew);
        fputc ('\r', filePointerNew);
        fputc ('\n', filePointerNew);
    }

    StrPrint (StrP);
}

void PrintSymbols (struct Strings* StrP)
{
    for (size_t i = 0; i < StrP->fileSize; i++)
    {
        Putchar (StrP->textPointer[i]);
        printf (" (%d)\n", StrP->textPointer[i]);
    }
}

void Putchar (char a)
{
    switch (a)
        {
        case '\r':
            printf ("\\r");
            break;
        case '\n':
            printf ("\\n");
            break;
        case '\0':
            printf ("\\0");
            break;
        case '\t':
            printf ("\\t");
            break;
        default:
            putchar (a);
            break;
        }
}

