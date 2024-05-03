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
