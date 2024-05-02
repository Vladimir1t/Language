#include "..\include\parcer.h"

static FILE* error_file = fopen ("Frontend\\log\\input_output_error.txt", "w");

static int add_node_in_graph (struct Node* node, FILE* file_graph, size_t* node_num);

static int add_connection_in_graph (struct Node* node, FILE* file_graph);

static void dump_node (struct Node *node);

int build_graphviz (struct Node* root, const char* file_name)
{
    CHECK_PTR (root);
    CHECK_PTR (file_name);

    size_t node_num = 0;
    if (root == NULL)
        return ERROR;

    FOPEN (file_graph, file_name, "w");

    fprintf (file_graph, "digraph LANGUAGE{\n"
                         "label = < LANGUAGE >;\n"
                         "bgcolor = \"#BAF0EC\";\n"
                         "node [shape = record ];\n"
                         "edge [style = filled ];\n");

        /*   create a tree in graphviz   */
    add_node_in_graph (root, file_graph, &node_num);
    add_connection_in_graph (root, file_graph);
    fprintf (file_graph, "}");

    fclose (file_graph);

    return SUCCESS;
}

int add_node_in_graph (struct Node* node, FILE* file_graph, size_t* node_num)
{
    CHECK_PTR (node);
    CHECK_PTR (file_graph);

    if (node->right == NULL && node->left == NULL)
    {
        if (node->type == T_VAR)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = YellowGreen, label = \"%s\" ];\n", *node_num, node->data.var);

        else if (node->type == T_NUM)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = YellowGreen, label = \"%d\" ];\n", *node_num, node->data.value);
    }
    else
    {
        if (node->type == T_OP)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"%c\" ];\n", *node_num, node->data.op);

        else if (node->type == T_OP_LONG)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"%s\" ];\n", *node_num, node->data.op_long);

        else if (node->type == T_KEY_W)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"%c\" ];\n", *node_num, node->data.key_w);

        else if (node->type == T_IF_)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"%s\" ];\n", *node_num, node->data.if_);

        else if (node->type == T_FUNC)
            fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"%s\" ];\n", *node_num, node->data.func);

        else if (node->type == T_SIGN)
        {
            printf ("[[%s]]\n",  node->data.sign);
            for (int i = 0; i < SIGN_NUM; i++)
                if (!strcmp (node->data.sign, array_sign[i].name))
                    fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"sign %s \" ];\n", *node_num, array_sign[i].code);
        }
    }

    node->num_in_tree = *node_num;
    if (node->left != NULL)
    {
        *node_num += 1;
        add_node_in_graph (node->left, file_graph, node_num);
    }

    if (node->right != NULL)
    {
        *node_num += 1;
        add_node_in_graph (node->right, file_graph, node_num);
    }

    return SUCCESS;
}

int add_connection_in_graph (struct Node* node, FILE* file_graph)
{
    CHECK_PTR (node);
    CHECK_PTR (file_graph);

    if (node->left != NULL)
    {
        fprintf (file_graph, "%d -> %d[ color = Peru ];\n", node->num_in_tree, (node->left)->num_in_tree);
        add_connection_in_graph (node->left, file_graph);
    }

    if (node->right != NULL && node->right->type != DEFUALT)
    {
        fprintf (file_graph, "%d -> %d[ color = Peru ];\n", node->num_in_tree, (node->right)->num_in_tree);
        add_connection_in_graph (node->right, file_graph);
    }

    return SUCCESS;
}


int tree_output (struct Node* node, FILE* file_output)
{
    CHECK_PTR (node);
    CHECK_PTR (file_output);

    dump_node (node);

    if (node->type == T_VAR)
        fprintf (file_output, "( \"%s\" ", node->data.var);

    else if (node->type == T_IF_)
        fprintf (file_output, "( \"%s\" ", node->data.if_);

    else if (node->type == T_FUNC)
        fprintf (file_output, "( \"%s\" ", node->data.func);

    else if (node->type == T_SIGN)
        fprintf (file_output, "( \"%s\" ", node->data.sign);

    else if (node->type == T_OP)
        fprintf (file_output, "( \"%c\" ", node->data.op);

    else if (node->type == T_KEY_W)
        fprintf (file_output, "( \"%c\" ", node->data.key_w);

    else if (node->type == T_CBR_O)
        fprintf (file_output, "( \"%c\" ", node->data.br_o);

    else if (node->type == T_CBR_C)
        fprintf (file_output, "( \"%c\" ", node->data.br_c);

    else if (node->type == T_NUM)
        fprintf (file_output, "( \"%d\" ", node->data.value);

    if (node->left != NULL)
        tree_output (node->left, file_output);

    if (node->right != NULL)
        tree_output (node->right, file_output);

    fprintf (file_output, ")");

    return 0;
}

void dump_node (struct Node *node)
{
    printf ("\n---------------NODE-------------\n");
    printf ("type - %d\n", node->type);

    if (node->type == T_NUM)
        printf ("# %d #", node->data.value);
    else if (node->type == T_OP)
        printf ("# %c #", node->data.op);
    else if (node->type == T_KEY_W)
        printf ("# %c #", node->data.key_w);
    else if (node->type == T_CBR_O)
        printf ("# %c #", node->data.br_o);
    else if (node->type == T_CBR_C)
        printf ("# %c #", node->data.br_c);
    else if (node->type == T_VAR)
        printf ("# %s #", node->data.var);

    printf ("\n--------------------------------\n");
}

void tree_dtor (struct Node* node)
{
    if (node->left != NULL)
        tree_dtor (node->left);

    if (node->right != NULL)
        tree_dtor (node->right);

    free (node);
}

void clean_buffer ()
{
    int symbol = 0;
    do
    {
        symbol = getchar ();
    }
    while (symbol != '\n' && symbol != EOF);
}



