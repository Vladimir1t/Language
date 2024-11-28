#include "Onegin.h"

int FileSize (FILE* const fPointer)
{
    assert (fPointer != NULL);

    int startPosition = ftell (fPointer);

    fseek (fPointer, 0, SEEK_END);
    size_t len = (size_t) ftell (fPointer);

    fseek (fPointer, startPosition, SEEK_SET);

    return len;
}

