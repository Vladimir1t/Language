#include "..\include\Onegin.h"

void ClearMemory (struct Strings* StrP)
{
    free (StrP->stringsP);
    free (StrP->textPointer);

    StrP->stringsP = NULL;
    StrP->textPointer = NULL;
}
