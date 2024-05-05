#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED

enum result
{
    SUCCESS       = 1,
    OPEN_ERROR    = 2,
    ERROR         = 3,
    NULL_PTR      = 4,
    REALLOC_ERROR = 5,
    CALLOC_ERROR  = 6
};

#endif // ERRORS_H_INCLUDED
