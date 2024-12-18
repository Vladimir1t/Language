#include "parcer.h"

static FILE* error_file = fopen ("Frontend/log/parcer_error.txt", "w");

#define IS_NUM isdigit    (text_data[ptr]) || (text_data[ptr] == '-' && (isdigit (text_data[ptr + 1])))
#define IS_STR_END        tokens->array_tokens[*ptr].type == KEY_W && tokens->array_tokens[*ptr].data.key_w == ';'
#define IS_BODY_END       tokens->array_tokens[*ptr].type == CBR_C && tokens->array_tokens[*ptr].data.br_c == '}'
#define IS_SIGN_EQUAL     value_1->type == T_VAR && tokens->array_tokens[*ptr].data.key_w == '='
#define IS_BRACKET_C      tokens->array_tokens[*ptr].type == BR_C && tokens->array_tokens[*ptr].data.br_c == ')'
#define IS_BRACKET_O      tokens->array_tokens[*ptr].type == BR_O && tokens->array_tokens[*ptr].data.br_o == '('
#define IS_OP_LONG_OR_VAR tokens->array_tokens[*ptr].type == VAR || tokens->array_tokens[*ptr].type == OP_L
#define IS_MUL_OR_DIV     tokens->array_tokens[*ptr].data.op == '*' || tokens->array_tokens[*ptr].data.op == '/'
#define IS_ADD_OR_SUB     tokens->array_tokens[*ptr].data.op == '+' || tokens->array_tokens[*ptr].data.op == '-'
#define IS_OP             text_data[ptr] == '+' || text_data[ptr] == '-' || text_data[ptr] == '*' || text_data[ptr] == '/' || text_data[ptr] == '^'
#define IS_SPEC_SYMBOL    text_data[ptr] == ' ' || text_data[ptr] == '\n' || text_data[ptr] == '\r'

static int get_tokens (struct Tokens* tokens, const char* text_data);

static struct Node* get_g         (struct Tokens* tokens, size_t* ptr);
static struct Node* get_equation  (struct Tokens* tokens, size_t* ptr);
static struct Node* get_assign    (struct Tokens* tokens, size_t* ptr);
static struct Node* get_condition (struct Tokens* tokens, size_t* ptr);
static struct Node* get_body      (struct Tokens* tokens, size_t* ptr);
static struct Node* get_e         (struct Tokens* tokens, size_t* ptr);
static struct Node* get_t         (struct Tokens* tokens, size_t* ptr);
static struct Node* get_p         (struct Tokens* tokens, size_t* ptr);
static struct Node* get_pow       (struct Tokens* tokens, size_t* ptr);
static struct Node* get_f         (struct Tokens* tokens, size_t* ptr);
static struct Node* get_n         (struct Tokens* tokens, size_t* ptr);
static struct Node* get_var       (struct Tokens* tokens, size_t* ptr);

static struct Node* syntax_error ();

static size_t file_size_measure (FILE* const file_p);

int get_database (struct Node** root, char* sourse_file)      // get data of tree in the following file
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

    struct Tokens tokens = {0};
    get_tokens (&tokens, text_data);

    size_t ptr = 0;
    *root = get_g (&tokens, &ptr);

    if (root == NULL)
        return ERROR;

    free (text_data);
    free (tokens.array_tokens);

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

    tokens->capacity = 20;
    CALLOC (tokens->array_tokens, struct Token, tokens->capacity);

    int ptr = 0;
    while (1)
    {
        if (tokens->size == tokens->capacity)
        {
            INCREASE_CAPACITY (tokens);
        }
        if (isalpha(text_data[ptr]))
        {
            char str[MAX_STR_SIZE] = {0};
            int i = 0, flag = 0;;
            while (isalpha (text_data[ptr]) || text_data[ptr] == '_')
                str[i++] = text_data[ptr++];

            if (!strcmp (str, "VAR"))              // if
            {
                ADD_IF_ (tokens);
            }
            else if (!strcmp (str, "Extra_time"))  // while
            {
                ADD_WHILE (tokens);
            }
            for (int j = 0; j < SIGN_NUM; j++)
            {
                if (!strcmp (str, array_sign[j].name))
                {
                    ADD_SIGN (tokens);
                }
            }
            for (int j = 0; j < FUNC_NUM; j++)
            {
                if (!strcmp (str, array_func[j].name))
                {
                    ADD_FUNCTION (tokens);
                }
            }
            if (!strcmp (str, "score"))       // =
            {
                flag = 1;
                ADD_TOKEN (KEY_W, key_w, '=');
            }
            else if (!strcmp (str, "match_start"))
            {
                flag = 1;
                ADD_TOKEN (CBR_O, br_o, '{');
            }
            else if (!strcmp (str, "match_end"))
            {
                flag = 1;
                ADD_TOKEN (CBR_C, br_c, '}');
            }
            else if (text_data[ptr] == '(')        // function
            {
                for (int j = 0; j < OP_NUM; j++)
                    if (!strcmp (str, array_op[j].name_std))
                    {
                        ADD_LONG_OP (tokens);
                    }
            }
            if (flag == 0)
            {
                ADD_VARIABLE (tokens);
            }
        }
        else if (IS_NUM)
        {
            ADD_NUMBER (tokens);
        }
        else if (IS_SPEC_SYMBOL)
            ptr++;

        else if (text_data[ptr] == '\0')
        {
            ADD_END_OF_FILE (tokens);
        }
        else
        {
            if (text_data[ptr] == '(')
            {
                ADD_TOKEN (BR_O, br_o, text_data[ptr]);
                ptr++;
            }
            else if (text_data[ptr] == ')')
            {
                ADD_TOKEN (BR_C, br_c, text_data[ptr]);
                ptr++;
            }
            else if (text_data[ptr] == ';')
            {
                ADD_TOKEN (KEY_W, key_w, text_data[ptr]);
                ptr++;
            }
            else if (IS_OP)
            {
                ADD_TOKEN (OP, op, text_data[ptr]);
                ptr++;
            }
            else
                return ERROR;
        }
    }

    return SUCCESS;
}

struct Node* get_g (struct Tokens* tokens, size_t* ptr)
{
    assert (tokens != NULL);
    assert (ptr != NULL);
    //printf ("G\n");

    struct Node* value = get_equation (tokens, ptr);

    if (tokens->array_tokens[*ptr].type == END)
        *ptr += 1;
    else
        return syntax_error ();

    return value;
}

struct Node* get_equation (struct Tokens* tokens, size_t* ptr)
{
    assert (tokens != NULL);
    assert (ptr != NULL);

    //printf ("Eq\n");
    unsigned char str_end    = ';';
    struct Node*  first_node = create_node (T_KEY_W, &str_end, get_assign (tokens, ptr), NULL);
    struct Node*  value_1    = first_node;
    struct Node*  value_2    = NULL;

    while ((IS_STR_END) || (IS_BODY_END))
    {
        *ptr += 1;
        value_2 = get_assign (tokens, ptr);
        if (value_2 == NULL)
            return first_node;
        if (IS_STR_END || (IS_BODY_END))
        {
            value_2 = create_node (T_KEY_W, &str_end, value_2, NULL);
            value_1->right = value_2;
            value_1 = value_2;
        }
    }

    return first_node;
}

struct Node* get_assign (struct Tokens* tokens, size_t* ptr)
{
    assert (tokens != NULL);
    assert (ptr != NULL);

    //printf ("Ass\n");
    struct Node* value_1 = NULL;
    struct Node* value_2 = NULL;
    struct Node* value_3 = NULL;

    value_1 = get_var (tokens, ptr);
    if (value_1 == NULL)
        return NULL;

    if (IS_SIGN_EQUAL)
    {
        unsigned char equal = '=';
        *ptr += 1;
        value_2 = get_e (tokens, ptr);

        if (value_2 == NULL)
            return NULL;

        value_1 = create_node (T_KEY_W, &equal, value_1, value_2);
    }
    else if (value_1->type == T_FUNC)
    {
        if (IS_BRACKET_O)
        {
            *ptr += 1;
            value_2 = get_var (tokens, ptr);
            if (value_2 == NULL || value_2->type != T_VAR)
                return syntax_error ();

            if (IS_BRACKET_C)
            {
                *ptr += 1;
                value_1->left = value_2;
                return value_1;
            }
            else
                return syntax_error ();
        }
        else
            return syntax_error ();
    }
    else if (value_1->type == T_IF_ || value_1->type == T_WHILE)
    {
        value_2 = get_condition (tokens, ptr);
        value_3 = get_body (tokens, ptr);

        value_1->left  = value_2;
        value_1->right = value_3;
    }
    else
        return syntax_error ();

    return value_1;
}

struct Node* get_condition (struct Tokens* tokens, size_t* ptr)
{
    assert (tokens != NULL);
    assert (ptr != NULL);

    //printf ("Cond\n");
    struct Node* value_1 = NULL;
    struct Node* value_2 = NULL;

    if (IS_BRACKET_O)
    {
        char str[MAX_STR_SIZE] = {0};
        *ptr += 1;
        value_1 = get_e (tokens, ptr);
        if (tokens->array_tokens[*ptr].type == SIGN)
        {
            strcpy (str, tokens->array_tokens[*ptr].data.sign);
            *ptr += 1;
        }
        else
            return syntax_error ();

        value_2 = get_e (tokens, ptr);

        if (IS_BRACKET_C)
            *ptr += 1;
        else
            return syntax_error ();

        value_1 = create_node (T_SIGN, str, value_1, value_2);
    }
    else
        return syntax_error ();

    return value_1;
}

struct Node* get_body (struct Tokens* tokens, size_t* ptr)
{
    assert (tokens != NULL);
    assert (ptr != NULL);

    //printf ("body\n");
    struct Node* value = NULL;

    if (tokens->array_tokens[*ptr].data.br_o == '{')
    {
        *ptr += 1;
        value = get_equation (tokens, ptr);

        if (tokens->array_tokens[*ptr].data.br_c != '}')
           // *ptr += 1;
        //else
            return syntax_error ();
    }
    else
        return syntax_error ();

    return value;
}

struct Node* get_e (struct Tokens* tokens, size_t* ptr)    //  + or -
{
    assert (tokens != NULL);
    assert (ptr != NULL);

    //printf ("E\n");
    struct Node* value_1 = get_t (tokens, ptr);

    while (IS_ADD_OR_SUB)
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
    assert (tokens != NULL);
    assert (ptr != NULL);

    //printf ("T\n");
    struct Node* value_1 = get_p (tokens, ptr);

    while (IS_MUL_OR_DIV)
    {
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
    assert (tokens != NULL);
    assert (ptr != NULL);

    //printf ("P\n");
    struct Node* value = NULL;
    if (IS_BRACKET_O)
    {
        *ptr += 1;
        value = get_e (tokens, ptr);

        if (IS_BRACKET_C)
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
    else if (IS_OP_LONG_OR_VAR)
        value = get_f (tokens, ptr);

    else
        value = get_n (tokens, ptr);

    return value;
}

struct Node* get_pow (struct Tokens* tokens, size_t* ptr)
{
    assert (tokens != NULL);
    assert (ptr != NULL);

    //printf ("Pow\n");
    struct Node* value = NULL;
    *ptr += 1;

    if (IS_BRACKET_O)
    {
        *ptr += 1;
        value = get_e (tokens, ptr);
        if (IS_BRACKET_C)
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
    assert (tokens != NULL);
    assert (ptr != NULL);

    //printf ("F\n");
    struct Node* value = NULL;

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
    assert (tokens != NULL);
    assert (ptr != NULL);

    //printf ("Var\n");
    struct Node* value = NULL;

    if (tokens->array_tokens[*ptr].type == VAR)
    {
        value = create_node (T_VAR, tokens->array_tokens[*ptr].data.var, NULL, NULL);
        *ptr += 1;
    }
    else if (tokens->array_tokens[*ptr].type == IF_)
    {
        value = create_node (T_IF_, tokens->array_tokens[*ptr].data.if_, NULL, NULL);
        *ptr += 1;
    }
    else if (tokens->array_tokens[*ptr].type == WHILE)
    {
        value = create_node (T_WHILE, tokens->array_tokens[*ptr].data.while_, NULL, NULL);
        *ptr += 1;
    }
    else if (tokens->array_tokens[*ptr].type == FUNC)
    {
        value = create_node (T_FUNC, tokens->array_tokens[*ptr].data.func, NULL, NULL);
        *ptr += 1;
    }

    return value;
}

struct Node* get_n (struct Tokens* tokens, size_t* ptr)   //determine numbers and variables
{
    assert (tokens != NULL);
    assert (ptr != NULL);

    //printf ("N\n");
    struct Node* value = (struct Node*) calloc (1, sizeof (struct Node));
    //printf ("[%d]\n", tokens->array_tokens[*ptr].data.value);
    if (tokens->array_tokens[*ptr].type == NUM)
    {
        value->type = T_NUM;
        value->data.value = tokens->array_tokens[*ptr].data.value;
        *ptr += 1;
    }
    else
        syntax_error ();

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
    assert (data != NULL);

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

        case T_CBR_O:
            new_node->data.br_o = *(unsigned char*) data;
            break;

        case T_CBR_C:
            new_node->data.br_c = *(unsigned char*) data;
            break;

        case T_VAR:
            new_node->data.var = (char*) calloc (MAX_STR_SIZE, sizeof (char));
            strcpy (new_node->data.var, (char*) data);
            break;

         case T_KEY_W:
            new_node->data.key_w = *(unsigned char*) data;
            break;

        case T_OP_LONG:
            new_node->data.op_long = (char*) calloc (MAX_OP_SIZE, sizeof (char));
            strcpy (new_node->data.op_long, (char*) data);
            break;

        case T_FUNC:
            new_node->data.func = (char*) calloc (MAX_STR_SIZE, sizeof (char));
            strcpy (new_node->data.func, (char*) data);
            break;

        case T_IF_:
            new_node->data.if_ = (char*) calloc (MAX_STR_SIZE, sizeof (char));
            strcpy (new_node->data.if_, (char*) data);
            break;

        case T_WHILE:
            new_node->data.while_ = (char*) calloc (MAX_STR_SIZE, sizeof (char));
            strcpy (new_node->data.while_, (char*) data);
            break;

        case T_SIGN:
            new_node->data.sign = (char*) calloc (MAX_OP_SIZE, sizeof (char));
            strcpy (new_node->data.sign, (char*) data);
            break;

        default:
            return NULL;
    }

    new_node->left  = left;
    new_node->right = right;

    return new_node;
}
