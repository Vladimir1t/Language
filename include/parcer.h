#ifndef PARCER_H_INCLUDED
#define PARCER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#include "..\library\errors.h"
#include "..\library\library_of_defines.h"
#include "..\library\commands.h"
#include "simplifier.h"

const int MAX_STR_SIZE = 20;

enum POSITION
{
    LEFT  = 0,
    RIGHT = 1,
    ROOT  = 2,
};

union node_data
{
    unsigned char operation;       // +, -, * etc
    unsigned char key_word;        // =, {, } etc
    double        value;           // double number
    char*         var;             // x, y, z, var etc
    char*         operation_long;  // sin, cos, ln etc
};

struct Node
{
    Class_type   type;
    node_data    data;
    struct Node* left;
    struct Node* right;
    int          num_in_tree;
};

int get_database (struct Node** root, char* sourse_file);

struct Node* get_g (const char* text_data);

struct Node* get_equation (char** ptr);

struct Node* get_ass (char** ptr);

struct Node* get_e (char** ptr);

struct Node* get_t (char** ptr);

struct Node* get_p (char** ptr);

struct Node* get_f (char** ptr);

struct Node* get_n (char** ptr);

struct Node* get_var (char** ptr);

struct Node* syntax_error ();

struct Node* create_node (Class_type type, void* data, struct Node* left, struct Node* right);

int build_graphviz (struct Node* root, const char* file_name);

void tree_dtor (struct Node* node);

void clean_buffer ();

#endif // PARCER_H_INCLUDED
