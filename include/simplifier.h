#ifndef SIMPLIFIER_H_INCLUDED
#define SIMPLIFIER_H_INCLUDED

#include "parcer.h"
#include "..\library\commands.h"

double calculator (struct Node* tree, int* is_var_in_subtree);

struct Node* copy_subtree (const struct Node* node);

void simplifier (struct Node* tree);

void simplifier_conv_of_const (struct Node* tree, int* changed);

void remove_neutral_elements (struct Node* tree, int* changed);

#endif // SIMPLIFIER_H_INCLUDED

