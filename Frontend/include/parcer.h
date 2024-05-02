#ifndef PARCER_H_INCLUDED
#define PARCER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "..\library\errors.h"
#include "..\library\library_of_defines.h"
#include "..\library\commands.h"
#include "simplifier.h"

enum POSITION
{
    LEFT  = 0,
    RIGHT = 1,
    ROOT  = 2,
};

enum token_t
{
    OP    = 1,
    KEY_W = 2,
    NUM   = 3,
    VAR   = 4,
    OP_L  = 5,
    BR_C  = 6,
    BR_O  = 7,
    CBR_C = 8,
    CBR_O = 9,
    IF_   = 10,
    FUNC  = 11,
    SIGN  = 12,
    END   = 13,
};

union node_data
{
    unsigned char op;       // +, -, * etc
    unsigned char key_w;    // =, ; etc
    unsigned char br_o;     // {, (
    unsigned char br_c;     // }, )
    int           value;    // double number
    char*         var;      // x, y, z, var etc
    char*         op_long;  // sin, cos, ln etc
    char*         func;     // in, out
    char*         if_;      // if
    char*         sign;     // <, >, ==, etc
};

struct Node
{
    Class_type   type;
    node_data    data;
    struct Node* left;
    struct Node* right;
    int          num_in_tree;
};

union Data
{
    unsigned char op;      // +, -, * etc
    unsigned char key_w;   // =, ; etc
    unsigned char br_o;    // {, (
    unsigned char br_c;    // }, )
    int           value;   // double number
    char*         var;     // x, y, z, var etc
    char*         op_long; // sin, cos, ln etc
    char*         func;    // in, out
    char*         if_;     // if
    char*         sign;    // <, >, ==, <= etc
    char          end;     // \0
};

struct Token
{
    token_t    type;
    union Data data;
};

struct Tokens
{
    struct Token* array_tokens;
    size_t        size;
    size_t        capacity;
};

int get_database (struct Node** root, char* sourse_file);

struct Node* create_node (Class_type type, void* data, struct Node* left, struct Node* right);

int build_graphviz (struct Node* root, const char* file_name);

int tree_output (struct Node* node, FILE* file_output);

void tree_dtor (struct Node* node);

void clean_buffer ();

#endif // PARCER_H_INCLUDED
