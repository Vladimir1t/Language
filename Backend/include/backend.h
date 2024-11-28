#ifndef TRANSLATOR_H_INCLUDED
#define TRANSLATOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "errors.h"
#include "library_of_defines.h"
#include "commands.h"
#include "stack.h"


enum POSITION
{
    LEFT  = 0,
    RIGHT = 1,
    ROOT  = 2,
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
    char*         while_;
};

struct Node
{
    Class_type   type;
    node_data    data;
    struct Node* left;
    struct Node* right;
    int          num_in_tree;
};

int get_database (struct Node** root, char* file_input);

int build_graphviz (struct Node* root, const char* file_name);

void clean_buffer ();

int run_translator (struct Node* root, char* output_file);

void tree_dtor (struct Node* node);

#endif // TRANSLATOR_H_INCLUDED
