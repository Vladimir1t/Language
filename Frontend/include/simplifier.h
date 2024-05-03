#ifndef SIMPLIFIER_H_INCLUDED
#define SIMPLIFIER_H_INCLUDED

#include "parcer.h"
#include "..\library\commands.h"

#define KEY_WORDS T_NUM && tree->type != DEFUALT && tree->type != T_KEY_W  &&  tree->type != T_IF_ && tree->type != T_FUNC && tree->type != T_KEY_W && tree->type != T_SIGN

int calculator (struct Node* tree, int* is_var_in_subtree);

struct Node* copy_subtree (const struct Node* node);

void simplifier (struct Node* tree);

void simplifier_conv_of_const (struct Node* tree, int* changed);

void remove_neutral_elements (struct Node* tree, int* changed);

#endif // SIMPLIFIER_H_INCLUDED

