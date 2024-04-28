#ifndef ERRORS_H_INCLUDED
#define ERRORS_H_INCLUDED

enum RESULT
{
    ERROR         = -1,
    SUCCESS       =  2,
    CALLOC_ERROR  =  3,
    REALLOC_ERROR =  4,
    OPEN_ERROR    =  5,
    NULL_PTR      = -2
};

#endif // ERRORS_H_INCLUDED
