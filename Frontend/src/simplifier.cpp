#include "..\include\simplifier.h"

static FILE* error_file = fopen ("Frontend\\log\\error_file_simplifier.txt", "w");

static Class_operation long_op_determinator (char* operation);

static int double_compare (double first_double, double second_double);

int calculator (struct Node* tree, int* is_var_in_subtree)
{
    if (tree == NULL)
        return ERROR;

    if (tree->type == T_VAR)  // if there is a variable in subtree
        *is_var_in_subtree = 1;

    if (*is_var_in_subtree == 1)
        return 10;

    if (tree->type == T_OP)
    {
        switch (tree->data.op)
        {
            case '^':
                return pow (calculator (tree->left, is_var_in_subtree), calculator (tree->right, is_var_in_subtree));
                break;

            case '+':
                return calculator (tree->left, is_var_in_subtree) + calculator (tree->right, is_var_in_subtree);
                break;

            case '-':
                return calculator (tree->left, is_var_in_subtree) - calculator (tree->right, is_var_in_subtree);
                break;

            case '*':
                return calculator (tree->left, is_var_in_subtree) * calculator (tree->right, is_var_in_subtree);
                break;

            case '/':
                return calculator (tree->left, is_var_in_subtree) / calculator (tree->right, is_var_in_subtree);
                break;

            default:
                return ERROR;
                break;
        }
    }
    else if (tree->type == T_OP_LONG)
    {
        switch (long_op_determinator (tree->data.op_long))
        {
            case OP_LN:
                return log (calculator (tree->left, is_var_in_subtree));
                break;

            case OP_SIN:
                return sin (calculator (tree->left, is_var_in_subtree));
                break;

            case OP_EXP:
                return exp (calculator (tree->left, is_var_in_subtree));
                break;

            case OP_COS:
                return cos (calculator (tree->left, is_var_in_subtree));
                break;

            case OP_TG:
                return tan (calculator (tree->left, is_var_in_subtree));

            case OP_SQRT:
                return sqrt (calculator (tree->left, is_var_in_subtree));

            default:
                return ERROR;
                break;
        }
    }

    return tree->data.value;
}

void simplifier (struct Node* tree)
{
    int changed = 1;
    while (changed > 0)
    {
        changed = 0;
        simplifier_conv_of_const (tree, &changed);
        remove_neutral_elements  (tree, &changed);
    }
}

struct Node* copy_subtree (const struct Node* node)
{
    if (node == NULL)
        fprintf (error_file, "<< NULL_PTR >>\n");

    struct Node* new_node = (struct Node*) calloc (1, sizeof (struct Node));

    memcpy (new_node, node, sizeof (struct Node));

    if (node->left != NULL)
        new_node->left = copy_subtree (node->left);
    if (node->right != NULL)
        new_node->right = copy_subtree (node->right);

    return new_node;
}

void simplifier_conv_of_const (struct Node* tree, int* changed)
{
    int is_var_in_subtree = 0;
    int value = calculator (tree, &is_var_in_subtree);

    if (is_var_in_subtree == 0 && tree->type != SPEC_WORDS)
    {
        tree->data.value = value;
        tree->type = T_NUM;

        SUBTREE_DTOR (tree->left);
        SUBTREE_DTOR (tree->right);
        *changed = 1;
    }
    if (tree->left != NULL)
    {
        simplifier_conv_of_const (tree->left, changed);
    }
    if (tree->right != NULL)
    {
        simplifier_conv_of_const (tree->right, changed);
    }
}

void remove_neutral_elements (struct Node* tree, int* changed)
{
    if (tree->data.op == '*')
    {
        if ((tree->left->type == T_NUM && tree->left->data.value == 0) ||
            (tree->right->type == T_NUM && tree->right->data.value == 0))
        {
            tree->type = T_NUM;
            tree->data.value = 0;

            SUBTREE_DTOR (tree->left);
            SUBTREE_DTOR (tree->right);

            *changed = 1;
        }
        else if (tree->left->type == T_NUM && tree->left->data.value == 1)
        {
            SUBTREE_DTOR (tree->left);
            memcpy (tree, tree->right, sizeof (struct Node));
            *changed = 1;
        }

        else if (tree->right->type == T_NUM && tree->right->data.value == 1)
        {
            SUBTREE_DTOR (tree->right);
            memcpy (tree, tree->left, sizeof (struct Node));
            *changed = 1;
        }
    }
    else if (tree->data.op == '+')
    {
        if (tree->left->type == T_NUM && tree->left->data.value == 0)
        {
            SUBTREE_DTOR (tree->left);
            memcpy (tree, tree->right, sizeof (struct Node));
            *changed += 1;
        }

        else if (tree->right->type == T_NUM && tree->right->data.value == 0)
        {
            SUBTREE_DTOR (tree->right);
            memcpy (tree, tree->left, sizeof (struct Node));
            *changed = 1;
        }
    }

    if (tree->left != NULL)
        remove_neutral_elements (tree->left, changed);

    if (tree->right != NULL)
        remove_neutral_elements (tree->right, changed);
}

Class_operation long_op_determinator (char* operation)
{
    if (!strcmp (operation, "ln"))
        return OP_LN;

    else if (!strcmp (operation, "sin"))
        return OP_SIN;

    else if (!strcmp (operation, "exp"))
        return OP_EXP;

    else if (!strcmp (operation, "cos"))
        return OP_COS;

    else if (!strcmp (operation, "sin"))
        return OP_SIN;

    else if (!strcmp (operation, "tg"))
        return OP_TG;

    else if (!strcmp (operation, "sqrt"))
        return OP_SQRT;
}
/*
int double_compare (double first_double, double second_double)
{
    const double EPSILON = 0.000001;
    return (fabs (first_double - second_double) <= EPSILON);
}*/

