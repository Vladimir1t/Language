#ifndef DSL_H_INCLUDED
#define DSL_H_INCLUDED

  /* Library of defines */

#define CHECK_PTR(ptr)                              \
    if (ptr == NULL)                                \
    {                                               \
        fprintf (error_file, "<< NULL ptr >>\n");   \
        return NULL_PTR;                            \
    }

#define REALLOC(elem, type, size)                            \
    do                                                       \
    {                                                        \
        void* clTmp = realloc (elem, (size) * sizeof (type));  \
        if (clTmp == NULL)                                   \
            return REALLOC_ERROR;                            \
        elem = (type*) clTmp;                                \
    }                                                        \
    while (0)

#define CALLOC(elem, type, size)                     \
    do                                               \
    {                                                \
        void* clTmp = calloc (size, sizeof (type));  \
        if (clTmp == NULL)                           \
            return CALLOC_ERROR;                     \
        elem = (type*) clTmp;                        \
    }                                                \
    while (0)

#define FOPEN(ptr, name, mode)       \
    FILE* ptr = fopen (name, mode);  \
    if (ptr == NULL)                 \
        return OPEN_ERROR;

#define NEXT_NODE(next)              \
    do                               \
    {                                \
        CALLOC (next, Node, 1);      \
    }                                \
    while(0)

#define SUBTREE_DTOR(node)      \
    do                          \
    {                           \
        free(node);             \
        node = NULL;            \
    }                           \
    while (0)

#define ADD_DIFF(node)                                                                     \
    {                                                                                      \
        unsigned char add = '+';                                                           \
        return create_node (T_OP, &add, diff (node->left, var), diff (node->right, var));  \
    }

#define SUB_DIFF(node)                                                                     \
    {                                                                                      \
        unsigned char sub = '-';                                                           \
        return create_node (T_OP, &sub, diff (node->left, var), diff (node->right, var));  \
    }

#define MUL_DIFF(node)                                                       \
    {                                                                        \
        unsigned char mul = '*', add = '+';                                  \
                                                                             \
        struct Node* du = diff (node->left, var);                            \
        struct Node* u  = copy_subtree (node->left);                         \
                                                                             \
        struct Node* dv = diff (node->right, var);                           \
        struct Node* v  = copy_subtree (node->right);                        \
                                                                             \
        return create_node (T_OP, &add, create_node (T_OP, &mul, du, v),     \
                                        create_node (T_OP, &mul, u, dv));    \
    }

#define DIV_DIFF(node)                                                                      \
    {                                                                                       \
        unsigned char mul = '*', sub = '-', div = '/';                                      \
        struct Node* du = diff (node->left, var);                                           \
        struct Node* u  = copy_subtree (node->left);                                        \
        struct Node* dv = diff (node->right, var);                                          \
        struct Node* v  = copy_subtree (node->right);                                       \
                                                                                            \
        struct Node* nominator = create_node (T_OP, &sub, create_node (T_OP, &mul, du, v),  \
                                                          create_node (T_OP, &mul, u, dv)); \
        struct Node* denominator = create_node (T_OP, &mul, copy_subtree (u),               \
                                                            copy_subtree (u));              \
        return create_node (T_OP, &div, nominator, denominator);                            \
    }

#define LN_DIFF(node)                                                                                   \
    {                                                                                                   \
        unsigned char mul = '*', div = '/';                                                             \
        double value = 1;                                                                               \
        struct Node* val = copy_subtree (node->left);                                                   \
        struct Node* dln = create_node (T_OP, &div,  create_node (T_NUM, &value, NULL, NULL), val);     \
        return create_node (T_OP, &mul, dln, diff (node->left, var));                                   \
    }

#define SIN_DIFF(node)                                                                                  \
    {                                                                                                   \
        unsigned char mul = '*';                                                                        \
        char* cos = "cos";                                                                              \
        struct Node* val = copy_subtree (node->left);                                                   \
        struct Node* dsin = create_node (T_OP_LONG, cos, val, NULL);                                    \
        return create_node (T_OP, &mul, dsin, diff (node->left, var));                                  \
    }

#define COS_DIFF(node)                                                                                  \
    {                                                                                                   \
        printf ("diff cos\n");                                                                          \
        unsigned char mul = '*', sub = '-';                                                             \
        double value = 0;                                                                               \
        char* sin = "sin";                                                                              \
        struct Node* val_1 = copy_subtree (node->left);                                                 \
        struct Node* val_2 = create_node (T_OP_LONG, sin, val_1, NULL);                                 \
        struct Node* dcos = create_node (T_OP, &mul, val_2, diff (node->left, var));                    \
        return create_node (T_OP, &sub, create_node (T_NUM, &value, NULL, NULL), dcos);                 \
    }

#define EXP_DIFF(node)                                                                                  \
    {                                                                                                   \
        unsigned char mul = '*';                                                                        \
        Node* val = copy_subtree (node);                                                                \
        Node* res = create_node (T_OP, &mul, val, diff (node->left, var));                              \
        return res;                                                                                     \
    }

#

#endif // DSL_H_INCLUDED
