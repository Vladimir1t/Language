#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

const int MAX_OP_SIZE  = 5;
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
    T_FUNC    =  7,
    T_IF_     =  8,
    T_SIGN    =  9,
    T_WHILE   = 10,
    DEFUALT   = -1,
};

enum Class_operation
{
    OP_ADD  = 1,
    OP_SUB  = 2,
    OP_MUL  = 3,
    OP_DIV  = 4,
    OP_SIN  = 5,
    OP_COS  = 6,
    OP_TG   = 7,
    OP_CTG  = 8,
    OP_LN   = 9,
    OP_SH   = 10,
    OP_CH   = 11,
    OP_DEG  = 12,
    OP_EXP  = 13,
    OP_SQRT = 14,
};

enum SIGNS
{
    B  = 1, // >
    A  = 2, // <
    E  = 3, // ==
    BE = 4, // <=
    AE = 5, // >=
    NE = 6, // !=
};

struct element
{
   char name[MAX_OP_SIZE];
   int  code;
};

static struct element array_op[] =
{
    {"+",    OP_ADD},
    {"-",    OP_SUB},
    {"*",    OP_MUL},
    {"/",    OP_DIV},
    {"sin",  OP_SIN},
    {"cos",  OP_COS},
    {"tg",   OP_TG},
    {"ctg",  OP_CTG},
    {"ln",   OP_LN},
    {"sh",   OP_SH},
    {"ch",   OP_CH},
    {"^",    OP_DEG},
    {"exp",  OP_EXP},
    {"sqrt", OP_SQRT}
};
const int OP_NUM = sizeof (array_op) / sizeof (array_op[0]);

static struct element array_func[] =
{
    {"in",  1},
    {"out", 2},
};
const int FUNC_NUM = sizeof (array_func) / sizeof (array_func[0]);

struct elem_sign
{
    char name[MAX_OP_SIZE];
    char code[MAX_OP_SIZE];
};
static struct elem_sign array_sign[] =
{
    {"<",  "b"},
    {">",  "a"},
    {"==", "e"},
    {"<=", "be"},
    {">=", "ae"},
    {"!=", "ne"}
};
const int SIGN_NUM = sizeof (array_sign) / sizeof (array_sign[0]);

struct Variable
{
    char name[MAX_STR_SIZE];
    //int  value;
    int  adr;
};

struct Variables
{
    struct Variable* array_var;
    int              size;
    int              capacity;
};

//static int var_num = sizeof (array_vr) / sizeof (array_vr[0]);

#endif // COMMANDS_H_INCLUDED
