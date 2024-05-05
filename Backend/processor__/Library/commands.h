#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

const size_t COMMAND_LEN = 5;

enum ARGUMENT
{
    HAS_ARG = 1,
    NO_ARG  = 0,
};

enum COMMANDS
{
    PUSH  = 1,
    ADD   = 2,
    SUB   = 3,
    DIV   = 4,
    MUL   = 5,
    OUT   = 6,
    IN    = 7,
    POP   = 8,
    HLT   = -1,
    JMP   = 37,
    JB    = 38,
    JBE   = 39,
    JA    = 40,
    JAE   = 41,
    JE    = 42,
    JNE   = 43,
    CALL  = 21,
    RET   = 22,
    SQRT  = 19

};

struct operation
{
    char command[COMMAND_LEN];
    int  num;
    int  argMod;
};

struct operation AllCommands[] =
{
    {"push", PUSH, HAS_ARG},
    {"add",  ADD,  NO_ARG},
    {"sub",  SUB,  NO_ARG},
    {"div",  DIV,  NO_ARG},
    {"mul",  MUL,  NO_ARG},
    {"out",  OUT,  NO_ARG},
    {"in",   IN,   NO_ARG},
    {"pop",  POP,  HAS_ARG},
    {"hlt",  HLT,  NO_ARG},
    {"jmp",  JMP,  HAS_ARG},
    {"jb",   JB,   HAS_ARG},
    {"jbe",  JBE,  HAS_ARG},
    {"ja",   JA,   HAS_ARG},
    {"jae",  JAE,  HAS_ARG},
    {"je",   JE,   HAS_ARG},
    {"jne",  JNE,  HAS_ARG},
    {"call", CALL, HAS_ARG},
    {"ret",  RET,  NO_ARG},
    {"sqrt", SQRT, NO_ARG}
};

const size_t COMMANDS_NUM = sizeof (AllCommands) / sizeof (AllCommands[0]);

#endif // COMMANDS_H_INCLUDED
