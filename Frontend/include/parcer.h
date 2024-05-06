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

#define IS_STR_END tokens->array_tokens[*ptr].type == KEY_W && tokens->array_tokens[*ptr].data.key_w == ';'
#define IS_SIGN_EQUAL value_1->type == T_VAR && tokens->array_tokens[*ptr].data.key_w == '='
#define IS_BRACKET_C tokens->array_tokens[*ptr].type == BR_C && tokens->array_tokens[*ptr].data.br_c == ')'
#define IS_BRACKET_O tokens->array_tokens[*ptr].type == BR_O && tokens->array_tokens[*ptr].data.br_o == '('
#define IS_OP_LONG_OR_VAR tokens->array_tokens[*ptr].type == VAR || tokens->array_tokens[*ptr].type == OP_L
#define IS_MUL_OR_DIV tokens->array_tokens[*ptr].data.op == '*' || tokens->array_tokens[*ptr].data.op == '/'
#define IS_ADD_OR_SUB tokens->array_tokens[*ptr].data.op == '+' || tokens->array_tokens[*ptr].data.op == '-'
#define IS_OP text_data[ptr] == '+' || text_data[ptr] == '-' || text_data[ptr] == '*' || text_data[ptr] == '/' || text_data[ptr] == '^'
#define IS_SPEC_SYMBOL text_data[ptr] == ' ' || text_data[ptr] == '\n' || text_data[ptr] == '\r'

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
    char*         while_;   // while 
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
    char*         while_;  // while
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
