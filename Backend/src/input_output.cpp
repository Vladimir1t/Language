#include "..\include\translator.h"

static FILE* error_file = fopen ("log\\error_file_input_output.txt", "w");

static int construct_data_nodes (struct Node* root, char* text_data, size_t file_size);
static int get_node_data (char* text_data, int* i, struct Node* node);
static size_t file_size_measure (FILE* const file_p);

static int add_node_in_graph (struct Node* node, FILE* file_graph, size_t* node_num);
static int add_connection_in_graph (struct Node* node, FILE* file_graph);

static void dump_node (struct Node *node);

int get_database (struct Node** root, char* file_input)   // get data of tree in the following file
{
    CALLOC (*root, struct Node, 1);
    //CALLOC ((*root)->data, char, DATA_SIZE);

    FOPEN (file_p, file_input, "rb");
    size_t file_size = file_size_measure (file_p);        // measures the size of a text
    printf ("size of file: %d\n", file_size);

    char* text_data = NULL;
    CALLOC (text_data, char, file_size + 1);

    size_t factual_size = fread (text_data, sizeof (char), file_size, file_p);
    if (factual_size != file_size)
    {
        fprintf (error_file, "<< Error. Wrong size of data_file >>\n");
        return ERROR;
    }
    fclose (file_p);

    for (int i = 0; i < file_size; i++)
        printf ("%c", text_data[i]);

    construct_data_nodes (*root, text_data, file_size);

    return SUCCESS;
}

int construct_data_nodes (struct Node* root, char* text_data, size_t file_size)
{
    struct Node* prev_node = NULL;
    struct stack stk       = {0};

    printf (" DATA_BASE \n");
    stack_ctor (&stk, 2);
    for (int i = 0, position = ROOT; i < file_size; i++)
    {
        if (text_data[i] == '{' && position == ROOT)  // add a root of the tree
        {
            char elem[MAX_OP_SIZE] = {0};

            SKIP_SYMBOLS (text_data[i]);

            GET_NODE_TYPE (root);

            SKIP_SYMBOLS (text_data[i]);

            get_node_data (text_data, &i, root);

            stack_push (&stk, (void**) &root);
            stack_push (&stk, (void**) &root);
            position = LEFT;
        }

        if (text_data[i] == '{')                      // add node in tree
        {
            char         elem[MAX_OP_SIZE] = {0};
            struct Node* node              = NULL;

            CALLOC (node, struct Node, 1);

            SKIP_SYMBOLS (text_data[i]);

            GET_NODE_TYPE (node);

            SKIP_SYMBOLS (text_data[i]);

            get_node_data (text_data, &i, node);

            stack_pop (&stk, (void**) &prev_node);

            if (position == LEFT)                     // add node as a left leaf
                prev_node->left = node;
            else                                      // add node as a right leaf
                prev_node->right = node;

            while (text_data[i] != '{' && text_data[i] != '}')
            {
                i++;
            }
            if (text_data[i] == '{')                  // next node will be in a left position
            {
                stack_push (&stk, (void**) &node);
                stack_push (&stk, (void**) &node);
                position = LEFT;
            }
            i--;
        }
        else if (text_data[i] == '}')                 // if a next node will be, it will stay in a right position
        {
            position = RIGHT;
        }
    }
    printf ("\n");
    stack_dtor (&stk);

    return SUCCESS;
}

int get_node_data (char* text_data, int* i, struct Node* node)
{
    switch (node->type)
    {
        case T_NUM:
            while ('0' <= text_data[*i] && text_data[*i] <= '9')
            {
                int prev_num = node->data.value;
                node->data.value = prev_num * 10 + (text_data[*i] - '0');
                *i += 1;
            }
            printf ("num [%d]\n", node->data.value);
            break;

        case T_VAR:
            {
                char str[MAX_STR_SIZE] = {0};
                int j = 0;
                while (isalpha (text_data[*i]))
                {
                    str[j++] = text_data[*i];
                    *i += 1;
                }
                CALLOC (node->data.var, char, MAX_STR_SIZE);
                strcpy (node->data.var, str);
                printf ("var [%s]\n", node->data.var);
                break;
            }
        case T_OP:
            node->data.op = text_data[*i];
            *i += 1;
            printf ("op [%c]\n", node->data.op);
            break;

        case T_OP_LONG:
            {
                char str[MAX_STR_SIZE] = {0};
                int j = 0;
                while (isalpha (text_data[*i]))
                {
                    str[j++] = text_data[*i];
                    *i += 1;
                }
                CALLOC (node->data.op_long, char, MAX_STR_SIZE);
                strcpy (node->data.op_long, str);
                printf ("op_long [%s]\n", node->data.op_long);
                break;
            }
        case T_FUNC:
            {
                char str[MAX_STR_SIZE] = {0};
                int j = 0;
                while (isalpha (text_data[*i]))
                {
                    str[j++] = text_data[*i];
                    *i += 1;
                }
                CALLOC (node->data.func, char, MAX_STR_SIZE);
                strcpy (node->data.func, str);
                printf ("func [%s]\n", node->data.func);
                break;
            }
        case T_KEY_W:
            node->data.key_w = text_data[*i];
            *i += 1;
            printf ("key_w [%c]\n", node->data.key_w);
            break;

        case T_CBR_O:
            node->data.br_o = text_data[*i];
            *i += 1;
            printf ("br_o [%c]\n", node->data.br_o);
            break;

        case T_CBR_C:
            node->data.br_c = text_data[*i];
            *i += 1;
            printf ("br_c [%c]\n", node->data.br_c);
            break;

        case T_IF_:
            {
                char str[MAX_STR_SIZE] = {0};
                int j = 0;
                while (isalpha (text_data[*i]))
                {
                    str[j++] = text_data[*i];
                    *i += 1;
                }
                CALLOC (node->data.if_, char, MAX_OP_SIZE);
                strcpy (node->data.if_, str);
                printf ("if [%s]\n", node->data.if_);
                break;
            }

        case T_SIGN:
            {
                char str[MAX_OP_SIZE] = {0};
                int j = 0;
                while ('<' <= text_data[*i] && text_data[*i] <= '>' && j < 2)
                {
                    str[j++] = text_data[*i];
                    *i += 1;
                }
                CALLOC (node->data.sign, char, MAX_OP_SIZE);
                strcpy (node->data.sign, str);
                printf ("sign [%s]\n", node->data.sign);
                break;
            }

        case DEFUALT:
            {
                //printf ("def\n");
                char str[MAX_STR_SIZE] = {0};
                int j = 0;
                while (isalpha (text_data[*i]))
                {
                    str[j++] = text_data[*i];
                    *i += 1;
                }
                printf ("def [%s]\n", str);
                break;
            }
    }

    return SUCCESS;
}

size_t file_size_measure (FILE* const file_p)
{
    assert (file_p != NULL);

    int start_position = ftell (file_p);
    fseek (file_p, 0, SEEK_END);
    size_t len = (size_t) ftell (file_p);

    fseek (file_p, start_position, SEEK_SET);

    return len;
}

int build_graphviz (struct Node* root, const char* file_name)
{
    CHECK_PTR (root);
    CHECK_PTR (file_name);

    size_t node_num = 0;
    if (root == NULL)
        return ERROR;

    FOPEN (file_graph, file_name, "w");

    fprintf (file_graph, "digraph LANGUAGE{\n"
                         "label = < Translator >;\n"
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
            PRINT_GR_LEAF ("%s", var);
        else if (node->type == T_NUM)
            PRINT_GR_LEAF ("%d", value);
        else if (node->type == T_FUNC)
            PRINT_GR_LEAF ("%s ()", func);
        else if (node->type == DEFUALT)
        {
            char* null = "null";
            PRINT_GR_SIGN ("%s", null);
        }
    }
    else
    {
        if (node->type == T_OP)
            PRINT_GR ("%c", op);
        else if (node->type == T_OP_LONG)
            PRINT_GR ("%s", op_long);
        else if (node->type == T_KEY_W)
            PRINT_GR ("%c", key_w);
        else if (node->type == T_IF_)
            PRINT_GR ("%s", if_);
        else if (node->type == T_FUNC)
            PRINT_GR ("%s ()", func);
        else if (node->type == T_SIGN)
            //printf ("[[%s]]\n",  node->data.sign);
            for (int i = 0; i < SIGN_NUM; i++)
                if (!strcmp (node->data.sign, array_sign[i].name))
                    PRINT_GR_SIGN ("sign %s", array_sign[i].code);
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

    if (node->right != NULL)
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
        fprintf (file_output, "{ #%d# #%s# ", node->type, node->data.var);
    else if (node->type == T_IF_)
        fprintf (file_output, "{ #%d# #%s# ", node->type, node->data.if_);
    else if (node->type == T_FUNC)
        fprintf (file_output, "{ #%d# #%s# ", node->type, node->data.func);
    else if (node->type == T_SIGN)
        fprintf (file_output, "{ #%d# #%s# ", node->type, node->data.sign);
    else if (node->type == T_OP)
        fprintf (file_output, "{ #%d# #%c# ", node->type, node->data.op);
    else if (node->type == T_KEY_W)
        fprintf (file_output, "{ #%d# #%c# ", node->type, node->data.key_w);
    else if (node->type == T_CBR_O)
        fprintf (file_output, "{ #%d# #%c# ", node->type, node->data.br_o);
    else if (node->type == T_CBR_C)
        fprintf (file_output, "{ #%d# #%c# ", node->type, node->data.br_c);
    else if (node->type == T_NUM)
        fprintf (file_output, "{ #%d# #%d# ", node->type, node->data.value);
    if (node->left != NULL)
        tree_output (node->left, file_output);
    if (node->right != NULL)
        tree_output (node->right, file_output);

    fprintf (file_output, "}");

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

    else if (node->type == T_IF_)
        printf ("# %s #", node->data.if_);
    else if (node->type == T_FUNC)
        printf ("# %s #", node->data.func);
    else if (node->type == T_SIGN)
    {
        printf ("ok\n");
        printf ("# %s #", node->data.sign);
    }

    printf ("\n--------------------------------\n");
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

void tree_dtor (struct Node* node)
{
    if (node->left != NULL)
        tree_dtor (node->left);

    if (node->right != NULL)
        tree_dtor (node->right);

    free (node);
}
