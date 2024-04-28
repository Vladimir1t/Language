#include "..\include\parcer.h"

static FILE* error_file = fopen ("log\\parcer_error.txt", "w");

static int get_tokens (struct Tokens* tokens, const char* text_data);

static size_t file_size_measure (FILE* const file_p);

static int add_node_in_graph (struct Node* node, FILE* file_graph, size_t* node_num);

static int add_connection_in_graph (struct Node* node, FILE* file_graph);

static void skip_symbols (char** ptr);

int get_database (struct Node** root, char* sourse_file)       // get data of tree in the following file
{
    CHECK_PTR (root);
    CHECK_PTR (sourse_file);

    CALLOC (*root, struct Node, 1);
    FOPEN (file_p, sourse_file, "rb");
    size_t file_size = file_size_measure (file_p);            // measures the size of a text
    char* text_data = NULL;
    CALLOC (text_data, char, file_size + 1);

    size_t factual_size = fread (text_data, sizeof (char), file_size, file_p);
    if (factual_size != file_size)
    {
        fprintf (error_file, "<< Error. Wrong size of data_file >>\n");
        return ERROR;
    }
    fclose (file_p);
    //text_data[file_size] = '$'; //

    struct Tokens tokens = {0};
    get_tokens (&tokens, text_data);

    //for (int i = 0; i < tokens.size; i++)
    //    printf ("..%d..\n", tokens.array_tokens[i].type);

    size_t ptr = 0;
    *root = get_g (&tokens, &ptr);

    if (root == NULL)
        return ERROR;

    return SUCCESS;
}

size_t file_size_measure (FILE* const file_p)
{
    CHECK_PTR (file_p);

    int start_position = ftell (file_p);
    fseek (file_p, 0, SEEK_END);
    size_t len = (size_t) ftell (file_p);
    fseek (file_p, start_position, SEEK_SET);

    return len;
}

int get_tokens (struct Tokens* tokens, const char* text_data)
{
    CHECK_PTR (tokens);
    CHECK_PTR (text_data);

    tokens->capacity = 19;
    CALLOC (tokens->array_tokens, struct Token, tokens->capacity);

    int ptr = 0;
    while (1)
    {
        if (tokens->size == tokens->capacity)
        {
            tokens->capacity *= 2;
            REALLOC (tokens->array_tokens, struct Token, tokens->capacity);
        }
        if (isalpha(text_data[ptr]))
        {
            char str[MAX_STR_SIZE] = {0};
            int i = 0;
            while (isalpha (text_data[ptr]))
                str[i++] = text_data[ptr++];

            if (text_data[ptr] == '(')             // function
            {
                for (int j = 0; j < OP_NUM; j++)
                    if (!strcmp (str, array_op[j].name))
                    {
                        tokens->array_tokens[tokens->size].type = OP_L;
                        CALLOC (tokens->array_tokens[tokens->size].data.op_long, char, MAX_STR_SIZE);
                        strcpy (tokens->array_tokens[tokens->size].data.op_long, str);
                        printf ("[%s]\n", tokens->array_tokens[tokens->size].data.op_long);
                        tokens->size += 1;
                    }
                //
            }
            else
            {
                tokens->array_tokens[tokens->size].type = VAR;
                CALLOC (tokens->array_tokens[tokens->size].data.var, char, MAX_STR_SIZE);
                strcpy (tokens->array_tokens[tokens->size].data.var, str);
                printf ("var [%s]\n", tokens->array_tokens[tokens->size].data.var);
                printf ("..%d..\n", tokens->array_tokens[tokens->size].type);

                tokens->size += 1;
            }
        }
        else if (isdigit (text_data[ptr]))
        {
            tokens->array_tokens[tokens->size].type = NUM;
            tokens->array_tokens[tokens->size].data.value = 0; //
            while ('0' <= text_data[ptr] && text_data[ptr] <= '9')
            {
                //printf ("ok\n");
                //printf ("..%c..\n", text_data[ptr]);
                //if (sign == '+')
                tokens->array_tokens[tokens->size].data.value = tokens->array_tokens[tokens->size].data.value * 10 + (text_data[ptr] - '0');

                ptr++;
            }

            printf ("[%d]\n", tokens->array_tokens[tokens->size].data.value);
            tokens->size += 1;
        }
        else if (text_data[ptr] == ' ')
            ptr++;

        else if (text_data[ptr] == '\0')
        {
            tokens->array_tokens[tokens->size].type = END;
            tokens->array_tokens[tokens->size].data.br_o = '\0';
            printf ("[%c]\n", tokens->array_tokens[tokens->size].data.end);
            tokens->size += 1;
            break;
        }

        else
        {
            /*switch (text_data[ptr])
            {
                case '{':*/
                if (text_data[ptr] == '{')
                {
                    tokens->array_tokens[tokens->size].type = CBR_O;
                    tokens->array_tokens[tokens->size].data.br_o = '{';
                    printf ("[%c]\n", tokens->array_tokens[tokens->size].data.br_o);
                    tokens->size += 1;
                    ptr++;
                    //break;
                }

                else if (text_data[ptr] == '}')
                {
                    tokens->array_tokens[tokens->size].type = CBR_C;
                    tokens->array_tokens[tokens->size].data.br_c = '}';
                    printf ("[%c]\n", tokens->array_tokens[tokens->size].data.br_c);
                    tokens->size += 1;
                    ptr++;
                    //break;
                }

                else if (text_data[ptr] == '(')
                {
                    tokens->array_tokens[tokens->size].type = BR_O;
                    tokens->array_tokens[tokens->size].data.br_c = '(';
                    printf ("[%c]\n", tokens->array_tokens[tokens->size].data.br_o);
                    tokens->size += 1;
                    ptr++;
                    //break;
                }

                else if (text_data[ptr] == ')')
                {
                    tokens->array_tokens[tokens->size].type = BR_C;
                    tokens->array_tokens[tokens->size].data.br_c = ')';
                    printf ("[%c]\n", tokens->array_tokens[tokens->size].data.br_c);
                    tokens->size += 1;
                    ptr++;
                    //break;
                }

                else if (text_data[ptr] == ';')
                {
                    tokens->array_tokens[tokens->size].type = KEY_W;
                    tokens->array_tokens[tokens->size].data.br_c = text_data[ptr];
                    printf ("[%c]\n", tokens->array_tokens[tokens->size].data.key_w);
                    //tokens->size += 1;
                    //ptr++;
                    tokens->size += 1;
                    ptr++;
                    //break;
                }

                else if (text_data[ptr] == '=')
                {
                    tokens->array_tokens[tokens->size].type = KEY_W;
                    tokens->array_tokens[tokens->size].data.br_c = text_data[ptr];
                    printf ("[%c]\n", tokens->array_tokens[tokens->size].data.key_w);
                    //tokens->size += 1;
                    //ptr++;
                    tokens->size += 1;
                    ptr++;
                    //break;
                }

                else if (text_data[ptr] == '+' || text_data[ptr] == '-' || text_data[ptr] == '*' || text_data[ptr] == '/' || text_data[ptr] == '^')
                {
                    tokens->array_tokens[tokens->size].type = OP;
                    tokens->array_tokens[tokens->size].data.op = text_data[ptr];
                    printf ("[%c]\n", tokens->array_tokens[tokens->size].data.op);
                    tokens->size += 1;
                    ptr++;
                   // break;
                }

                else
                    ptr++;

            //}
            //printf ("jjlk %c\n", text_data[ptr]);
        }
    }

    return SUCCESS;
}

void skip_symbols (char** ptr)
{
    while (**ptr == ' ' || **ptr == '\n' || **ptr == '\r')
        *ptr += 1;
}

struct Node* get_g (struct Tokens* tokens, size_t* ptr)
{
    printf ("G\n");

    struct Node* value = get_equation (tokens, ptr);

    if (tokens->array_tokens[*ptr].type == END)
        *ptr += 1;

    else
        return syntax_error ();

    return value;
}

struct Node* get_equation (struct Tokens* tokens, size_t* ptr)
{
    printf ("Eq\n");
    unsigned char str_end = ';';
    struct Node*  value_1 = get_ass (tokens, ptr);
    struct Node*  value_2 = NULL;
    printf ("first\n");
    while (tokens->array_tokens[*ptr].type == KEY_W && tokens->array_tokens[*ptr].data.key_w == ';')
    {
        *ptr += 1;
        struct Node* value_2 = get_ass (tokens, ptr);
        if (tokens->array_tokens[*ptr].type == KEY_W && tokens->array_tokens[*ptr].data.key_w == ';')
            value_1 = create_node (T_KEY_W, &str_end, value_1, value_2);
    }

    return value_1;
}

struct Node* get_ass (struct Tokens* tokens, size_t* ptr)
{
    printf ("Ass\n");
    struct Node* value_1 = NULL;
    struct Node* value_2 = NULL;
    unsigned char equal = '=';

    value_1 = get_var (tokens, ptr);
    if (value_1 == NULL)
        return NULL;

    if (tokens->array_tokens[*ptr].type == KEY_W && tokens->array_tokens[*ptr].data.key_w == '=')
        *ptr += 1;
    else
        return syntax_error ();

    value_2 = get_e (tokens, ptr);

    if (value_2 == NULL)
        return NULL;

    value_1 = create_node (T_KEY_W, &equal, value_1, value_2);

    return value_1;
}


struct Node* get_e (struct Tokens* tokens, size_t* ptr)    //  + or -
{
    printf ("E\n");
    struct Node* value_1 = get_t (tokens, ptr);

    while (tokens->array_tokens[*ptr].data.op == '+' || tokens->array_tokens[*ptr].data.op == '-')
    {
        const char op = tokens->array_tokens[*ptr].data.op;
        *ptr += 1;
        struct Node* value_2 = get_t (tokens, ptr);

        if (op == '+')
        {
            unsigned char add = '+';
            value_1 = create_node (T_OP, &add, value_1, value_2);
        }

        else if (op == '-')
        {
            unsigned char sub = '-';
            value_1 = create_node (T_OP, &sub, value_1, value_2);
        }
    }

    return value_1;
}

struct Node* get_t (struct Tokens* tokens, size_t* ptr)   //   * or /
{
    printf ("T\n");
    struct Node* value_1 = get_p (tokens, ptr);

    while (tokens->array_tokens[*ptr].data.op == '*' || tokens->array_tokens[*ptr].data.op == '/')
    {
        printf ("{*}\n");
        const char op = tokens->array_tokens[*ptr].data.op;
        *ptr += 1;
        struct Node* value_2 = get_p (tokens, ptr);

        if (op == '*')
        {
            unsigned char mul = '*';
            value_1 = create_node (T_OP, &mul, value_1, value_2);
        }

        else if (op == '/')
        {
            unsigned char div = '/';
            value_1 = create_node (T_OP, &div, value_1, value_2);
        }
    }

    return value_1;
}

struct Node* get_p (struct Tokens* tokens, size_t* ptr)   //  (, )  and determine functions
{
    printf ("P\n");
    struct Node* value = NULL;
    printf ("{%d}\n", tokens->array_tokens[*ptr].type);
    if (tokens->array_tokens[*ptr].data.br_o  == '(')
    {
        *ptr += 1;
        value = get_e (tokens, ptr);

        if (tokens->array_tokens[*ptr].data.br_c  == ')') // ?
        {
            *ptr += 1;
            if (tokens->array_tokens[*ptr].data.op == '^')
            {
                unsigned char pow = '^';
                struct Node* value_2 = get_pow (tokens, ptr);
                value = create_node (T_OP, &pow, value, value_2);
            }
        }
    }

    else if (tokens->array_tokens[*ptr].type == VAR || tokens->array_tokens[*ptr].type == OP_L)
        value = get_f (tokens, ptr);

    else
        value = get_n (tokens, ptr);

    return value;
}

struct Node* get_pow (struct Tokens* tokens, size_t* ptr)
{
    printf ("Pow\n");
    struct Node* value = NULL;
    *ptr += 1;

    if (tokens->array_tokens[*ptr].data.br_o == '(')
    {
        *ptr += 1;
        value = get_e (tokens, ptr);
        if (tokens->array_tokens[*ptr].data.br_c == ')')
        {
            *ptr += 1;
            return value;
        }
        else
            return syntax_error ();
    }
}

struct Node* get_f (struct Tokens* tokens, size_t* ptr)
{
    printf ("F\n");
    struct Node* value = NULL;
    //char str[MAX_STR_SIZE] = {0};
    //int i = 0;
    /*while (isalpha (**ptr))
    {
        str[i++] = **ptr;
        *ptr += 1;
    }

    if (**ptr == '(')             // function
    {
        for (int j = 0; j < OP_NUM; j++)
           if (!strcmp (str, array_op[j].name))
           {
                value = get_p (ptr);
                value = create_node (T_OP_LONG, array_op[j].name, value, NULL);
                return value;
           }
        return syntax_error ();
    }
    else if (isdigit (**ptr))
        return syntax_error ();*/
    if (tokens->array_tokens[*ptr].type == OP_L)
    {
        char* op_long = tokens->array_tokens[*ptr].data.op_long;
        *ptr += 1;
        value = get_p (tokens, ptr);
        value = create_node (T_OP_LONG, op_long, value, NULL);
    }

    else                        // variable
    {
        value = create_node (T_VAR, tokens->array_tokens[*ptr].data.var, NULL, NULL);
        *ptr += 1;
    }
    return value;
}

struct Node* get_var (struct Tokens* tokens, size_t* ptr)
{
    printf ("Var\n");
    struct Node* value = NULL;
    //char str[MAX_STR_SIZE] = {0};
    //int i = 0;

    /*while (isalpha (**ptr))
    {
        str[i++] = **ptr;
        *ptr += 1;
    }
    if (strlen (str) == 0)
        return NULL;

    else   // variable*/
    //tokens->array_tokens[*ptr].type = VAR;
    printf ("{%d}\n", tokens->array_tokens[*ptr].type);
    //printf ("{var %s}\n", tokens->array_tokens[*ptr].data.var);
    if (tokens->array_tokens[*ptr].type == VAR)
    {
        value = create_node (T_VAR, tokens->array_tokens[*ptr].data.var, NULL, NULL);
        *ptr += 1;
    }

    return value;
}

struct Node* get_n (struct Tokens* tokens, size_t* ptr)   //determine numbers and variables
{
    printf ("N\n");
    //const char* old_ptr = *ptr;
    printf ("{value %d}\n", tokens->array_tokens[*ptr].data.value);
    struct Node* value = (struct Node*) calloc (1, sizeof (struct Node));

    /*if (isdigit (**ptr))
    {
        value->type = T_NUM;

        char sign = '+';
        if (**ptr == '-')
            sign = '-';

        while ('0'<= **ptr && **ptr <= '9')
        {
            if (sign == '+')
                value->data.value = value->data.value * 10 + (**ptr - '0');
            else
                value->data.value = value->data.value * 10 - (**ptr - '0');
            *ptr += 1;
        }
    }
    else
        syntax_error ();

    if (*ptr == old_ptr)
        return syntax_error ();*/
    if (tokens->array_tokens[*ptr].type == NUM)
    {
        value->type = T_NUM;
        value->data.value = tokens->array_tokens[*ptr].data.value;
        *ptr += 1;
    }

    else
        syntax_error ();

    printf ("ok\n");
    return value;
}

struct Node* syntax_error ()
{
    printf ("Syntax Error\n");
    fprintf (error_file, "<< Syntax Error >>\n");

    return NULL;
}

struct Node* create_node (Class_type type, void* data, struct Node* left, struct Node* right)
{
    struct Node* new_node = (struct Node*) calloc (1, sizeof (struct Node));

    new_node->type = type;
    switch (type)
    {
        case T_NUM:
            new_node->data.value = *(int*) data;
            break;

        case T_OP:
            new_node->data.op = *(unsigned char*) data;
            break;

        case T_VAR:
            new_node->data.var = (char*) calloc (MAX_STR_SIZE, sizeof (char));
            strcpy (new_node->data.var, (char*) data);
            //printf ("[%s]\n", new_node->data.var);
            break;

         case T_KEY_W:
            new_node->data.key_w = *(unsigned char*) data;
            break;

        case T_OP_LONG:
            new_node->data.op_long = (char*) calloc (MAX_OP_SIZE, sizeof (char));
            strcpy (new_node->data.op_long, (char*) data);
            break;

        default:
            return NULL;
    }

    new_node->left  = left;
    new_node->right = right;

    return new_node;
}

int build_graphviz (struct Node* root, const char* file_name)
{
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

static int add_node_in_graph (struct Node* node, FILE* file_graph, size_t* node_num)
{
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

static int add_connection_in_graph (struct Node* node, FILE* file_graph)
{
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
    if (node->left != NULL)
        tree_output (node->left, file_output);

    //dump_node (node);
    if (node->type == T_VAR)
        fprintf (file_output, "( %c ", node->data.var);
    else if (node->type == T_OP)
        fprintf (file_output, " %c ", node->data.op);
    else if (node->type == T_NUM)
        fprintf (file_output, "( %lf ", node->data.value);

    fprintf (file_output, ")");

    if (node->right != NULL)
        tree_output (node->right, file_output);

    return 0;
}

void dump_node (struct Node *tree)
{
    printf ("\n---------------NODE-------------\n");
    printf ("type - %d\n", tree->type);

    if (tree->type == T_NUM)
        printf ("# %lf #", tree->data.value);
    else if (tree->type == T_OP)
        printf ("# %c #", tree->data.op);
    else if (tree->type == T_VAR)
        printf ("# %c #", tree->data.var);

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


