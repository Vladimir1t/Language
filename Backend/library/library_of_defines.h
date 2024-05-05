#ifndef DSL_H_INCLUDED
#define DSL_H_INCLUDED

  /* Library of defines */

#define CHECK_PTR(ptr)                              \
    if (ptr == NULL)                                \
    {                                               \
        fprintf (error_file, "<< NULL ptr >>\n");   \
        return NULL_PTR;                            \
    }

#define REALLOC(elem, type, size)                              \
    do                                                         \
    {                                                          \
        void* clTmp = realloc (elem, (size) * sizeof (type));  \
        if (clTmp == NULL)                                     \
            return REALLOC_ERROR;                              \
        elem = (type*) clTmp;                                  \
    }                                                          \
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

#define INCREASE_CAPACITY(...)                                              \
    variables->capacity *= 2;                                               \
    REALLOC (variables->array_var, struct Variable, variables->capacity);   \

#define ADD_LONG_OP(...)                                        \
    {                                                           \
        char str[MAX_STR_SIZE] = {0};                           \
        int j = 0;                                              \
        while (isalpha (text_data[*i]))                         \
        {                                                       \
            str[j++] = text_data[*i];                           \
            *i += 1;                                            \
        }                                                       \
        CALLOC (node->data.op_long, char, MAX_STR_SIZE);        \
        strcpy (node->data.op_long, str);                       \
    }

#define ADD_FUNCTION(...)                                           \
    {                                                               \
        char str[MAX_STR_SIZE] = {0};                               \
        int j = 0;                                                  \
        while (isalpha (text_data[*i]))                             \
        {                                                           \
            str[j++] = text_data[*i];                               \
            *i += 1;                                                \
        }                                                           \
        CALLOC (node->data.func, char, MAX_STR_SIZE);               \
        strcpy (node->data.func, str);                              \
    }

#define ADD_IF_(...)                                                               \
    tokens->array_tokens[tokens->size].type = IF_;                                 \
    CALLOC (tokens->array_tokens[tokens->size].data.if_, char, MAX_STR_SIZE);      \
    strcpy (tokens->array_tokens[tokens->size].data.if_, str);                     \
    printf ("if [%s]\n", tokens->array_tokens[tokens->size].data.if_);             \
    tokens->size += 1;

#define ADD_OP(...)                 \
    node->data.op = text_data[*i];  \
    *i += 1;                        \

#define ADD_VARIABLE(...)                                   \
    {                                                       \
        char str[MAX_STR_SIZE] = {0};                       \
        int j = 0;                                          \
        while (isalpha (text_data[*i]))                     \
        {                                                   \
            str[j++] = text_data[*i];                       \
            *i += 1;                                        \
        }                                                   \
        CALLOC (node->data.var, char, MAX_STR_SIZE);        \
        strcpy (node->data.var, str);                       \
    }

#define ADD_SIGN(...)                                                               \
    char str[MAX_OP_SIZE] = {0};                                                    \
    int i = 0;                                                                      \
    while ('<' <= text_data[ptr] && text_data[ptr] <= '>' && i < 2)                 \
        str[i++] = text_data[ptr++];                                                \
    if (!strcmp (str, "="))                                                         \
    {                                                                               \
        ADD_TOKEN (KEY_W, key_w, '=');                                              \
        printf ("key_w [%s]\n", str);                                               \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        tokens->array_tokens[tokens->size].type = SIGN;                             \
        CALLOC (tokens->array_tokens[tokens->size].data.sign, char, MAX_OP_SIZE);   \
        strcpy (tokens->array_tokens[tokens->size].data.sign, str);                 \
        printf ("sign [%s]\n", tokens->array_tokens[tokens->size].data.sign);       \
        tokens->size += 1;                                                          \
    }

#define ADD_NUMBER(...)                                                                               \
    tokens->array_tokens[tokens->size].type = NUM;                                                    \
    tokens->array_tokens[tokens->size].data.value = 0;                                                \
    int prev_num = 0;                                                                                 \
    while ('0' <= text_data[ptr] && text_data[ptr] <= '9')                                            \
    {                                                                                                 \
        prev_num = tokens->array_tokens[tokens->size].data.value;                                     \
        tokens->array_tokens[tokens->size].data.value = prev_num * 10 + (text_data[ptr] - '0');       \
        ptr++;                                                                                        \
    }                                                                                                 \
    printf ("[%d]\n", tokens->array_tokens[tokens->size].data.value);                                 \
    tokens->size += 1;

#define PRINT_GR_LEAF(spec, elem)                                                                                                                    \
    fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = YellowGreen, label = \"" spec "\" ];\n", *node_num, node->data.elem)     \

#define PRINT_GR(spec, elem)                                                                                                                   \
    fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"" spec "\" ];\n", *node_num, node->data.elem)      \

#define PRINT_GR_SIGN(spec, elem)                                                                                                               \
    fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"" spec "\" ];\n", *node_num, elem)                  \

#define SKIP_SYMBOLS(...)               \
    while (text_data[i] != '#')         \
        i++;                            \
    i++;

#define SEARCH_FOR_END_TOKEN(...)                           \
    while (text_data[i] != '{' && text_data[i] != '}')      \
        i++;


#define GET_NODE_TYPE(node)                 \
    int n = 0;                              \
    while (text_data[i] != '#')             \
        elem[n++] = text_data[i++];         \
    i++;                                    \
    sscanf (elem, "%d", &(node->type));     \
    //printf ("{%d}\n", node->type);

#define ADD_VAR_ASM(...)                                                    \
    int flag = 0;                                                           \
    for (int i = 0; i < variables->size; i++)                               \
        if (!strcmp (node->data.var, variables->array_var[i].name))         \
        {                                                                   \
            flag = 1;                                                       \
            printf ("var\n");                                               \
            fprintf (asm_file, "push [%d]\n", variables->array_var[i].adr); \
        }                                                                   \
    if (flag == 0)                                                          \
    {                                                                       \
        printf ("<< Undefined variable >>\n");                              \
        err = 1;                                                            \
        return ERROR;                                                       \
    }

#define ADD_OP_ASM(elem)                                                \
    if (node->left != NULL && node->left->type != DEFUALT)              \
        make_asm_code (node->left, variables, asm_file);                \
    if (node->left != NULL && node->right->type != DEFUALT)             \
        make_asm_code (node->right, variables, asm_file);               \
    fprintf (asm_file, elem);

#define ADD_OP_LONG_ASM(elem)                                   \
    if (node->left != NULL && node->left->type != DEFUALT)      \
        make_asm_code (node->left, variables, asm_file);        \
    fprintf (asm_file, elem);

#endif // DSL_H_INCLUDED
