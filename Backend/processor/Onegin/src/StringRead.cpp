#include "..\include\Onegin.h"

fileFormat StringsCount (struct Strings* StrP)
{
    assert (StrP->textPointer != NULL);
    fileFormat isR = UNDEFINED;

    for (size_t j = 0; j < StrP->fileSize + 1; j++)
    {
        if (StrP->textPointer[j] == '\r')
        {
            StrP->textPointer[j] = '\0';
            (StrP->nStrings)++;
            j++;
            continue;
        }
        else if (StrP->textPointer[j] == '\n')
        {
            StrP->textPointer[j] = '\0';
            (StrP->nStrings)++;
            continue;
        }
    }
    printf ("number of strings: %d\n", StrP->nStrings);

    char* elementP = strchr (StrP->textPointer, '\0');
    isR = (fileFormat) (StrP->nStrings > 1 && *(elementP + 1) == '\n');  // defines file format

    return isR;
}

void StringsPointerRead (struct Strings* StrP, fileFormat isR)
{
    assert (StrP->stringsP    != NULL);
    assert (StrP->textPointer != NULL);

    size_t n = 0;
    StrP->stringsP[n].pointer = StrP->textPointer;

    n++;

    for (size_t i = 0; i < StrP->fileSize; i++)
    {
        if (StrP->textPointer[i] == '\0' && n < StrP->nStrings)
        {
            if (isR == YES)
            {
                StrP->stringsP[n].pointer = StrP->textPointer + (i + 2);
                //printf ("strP: %s\n", StrP->stringsP[n].pointer);
                i++;
            }
            else if (isR == NO)
            {
                StrP->stringsP[n].pointer = StrP->textPointer + (i + 1);
                //printf ("strP: %s\n", StrP->stringsP[n].pointer);
            }
            //printf ("address of %d string: %d\n", n + 1, StrP->stringsP[n].pointer);
            n++;
        }
        else if (StrP->textPointer[i] == '#')
            StrP->textPointer[i] = '\0';
    }
}
