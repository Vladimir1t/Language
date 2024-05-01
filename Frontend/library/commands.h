#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

const int MAX_OP_SIZE  = 4;
const int MAX_STR_SIZE = 20;

enum Class_type
{
    T_NUM     =  0,
    T_VAR     =  1,
    T_OP      =  2,
    T_OP_LONG =  3,
    T_KEY_W   =  4,
    T_CBR_O   =  5,
    T_CBR_C   =  6,
    DEFUALT   = -1,
};

enum Class_operation
{
    OP_ADD = 1,
    OP_SUB = 2,
    OP_MUL = 3,
    OP_DIV = 4,
    OP_SIN = 5,
    OP_COS = 6,
    OP_TG  = 7,
    OP_CTG = 8,
    OP_LN  = 9,
    OP_SH  = 10,
    OP_CH  = 11,
    OP_DEG = 12,
    OP_EXP = 13
};

enum Class_key_word
{
    equal    = '=',
    skobe_op = '{',
    skobe_cl = '}',
    str_end  = ';',
};

struct operation
{
   char name[MAX_OP_SIZE];
   int  code;
};

static struct operation array_op[] =
{
    {"+",   OP_ADD},
    {"-",   OP_SUB},
    {"*",   OP_MUL},
    {"/",   OP_DIV},
    {"sin", OP_SIN},
    {"cos", OP_COS},
    {"tg",  OP_TG},
    {"ctg", OP_CTG},
    {"ln",  OP_LN},
    {"sh",  OP_SH},
    {"ch",  OP_CH},
    {"^",   OP_DEG},
    {"exp", OP_EXP}
};

const int OP_NUM = sizeof (array_op) / sizeof (array_op[0]);

struct variable
{
    char  name[MAX_STR_SIZE];
    int   value;
};

static struct variable array_vr[] =
{
    {"x", 0},
    {"y", 0},
    {"z", 0},
};

//static int var_num = sizeof (array_vr) / sizeof (array_vr[0]);

#endif // COMMANDS_H_INCLUDED
