#ifndef PARCER_H_INCLUDED
#define PARCER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "errors.h"
#include "library_of_defines.h"
#include "commands.h"
#include "simplifier.h"

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
