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
    tokens->capacity *= 2;                                                  \
    REALLOC (tokens->array_tokens, struct Token, tokens->capacity);

#define ADD_LONG_OP(...)                                                            \
    tokens->array_tokens[tokens->size].type = OP_L;                                 \
    CALLOC (tokens->array_tokens[tokens->size].data.op_long, char, MAX_STR_SIZE);   \
    strcpy (tokens->array_tokens[tokens->size].data.op_long, str);                  \
    tokens->size += 1;                                                              \
    break;

#define ADD_FUNCTION(...)                                                           \
    tokens->array_tokens[tokens->size].type = FUNC;                                 \
    CALLOC (tokens->array_tokens[tokens->size].data.func, char, MAX_STR_SIZE);      \
    strcpy (tokens->array_tokens[tokens->size].data.func, str);                     \
    tokens->size += 1;                                                              \
    break;

#define ADD_IF_(...)                                                               \
    tokens->array_tokens[tokens->size].type = IF_;                                 \
    CALLOC (tokens->array_tokens[tokens->size].data.if_, char, MAX_STR_SIZE);      \
    strcpy (tokens->array_tokens[tokens->size].data.if_, str);                     \
    tokens->size += 1;

#define ADD_VARIABLE(...)                                                       \
    tokens->array_tokens[tokens->size].type = VAR;                              \
    CALLOC (tokens->array_tokens[tokens->size].data.var, char, MAX_STR_SIZE);   \
    strcpy (tokens->array_tokens[tokens->size].data.var, str);                  \
    tokens->size += 1;

#define ADD_SIGN(...)                                                                                \
    char str[MAX_OP_SIZE] = {0};                                                                     \
    int i = 0;                                                                                       \
    while ((('<' <= text_data[ptr] && text_data[ptr] <= '>') || text_data[ptr] == '!') && i < 2)     \
        str[i++] = text_data[ptr++];                                                                 \
    if (!strcmp (str, "="))                                                                          \
    {                                                                                                \
        ADD_TOKEN (KEY_W, key_w, '=');                                                               \
    }                                                                                                \
    else                                                                                             \
    {                                                                                                \
        tokens->array_tokens[tokens->size].type = SIGN;                                              \
        CALLOC (tokens->array_tokens[tokens->size].data.sign, char, MAX_OP_SIZE);                    \
        strcpy (tokens->array_tokens[tokens->size].data.sign, str);                                  \
        tokens->size += 1;                                                                           \
    }

#define ADD_NUMBER(...)                                                                                      \
    tokens->array_tokens[tokens->size].type = NUM;                                                           \
    tokens->array_tokens[tokens->size].data.value = 0;                                                       \
    int prev_num = 0;                                                                                        \
    char num_sign = '+';                                                                                     \
    if (text_data[ptr] == '-')                                                                               \
    {                                                                                                        \
        num_sign = '-';                                                                                      \
        ptr += 1;                                                                                            \
    }                                                                                                        \
    while ('0' <= text_data[ptr] && text_data[ptr] <= '9')                                                   \
    {                                                                                                        \
        prev_num = tokens->array_tokens[tokens->size].data.value;                                            \
        if (num_sign == '+')                                                                                 \
            tokens->array_tokens[tokens->size].data.value = prev_num * 10 + (text_data[ptr] - '0');          \
        else                                                                                                 \
            tokens->array_tokens[tokens->size].data.value = prev_num * 10 - (text_data[ptr] - '0');          \
        ptr++;                                                                                               \
    }                                                                                                        \
    tokens->size += 1;

#define ADD_END_OF_FILE(...)                                            \
    tokens->array_tokens[tokens->size].type = END;                      \
    tokens->array_tokens[tokens->size].data.br_o = '\0';                \
    tokens->size += 1;                                                  \
    break;

#define ADD_TOKEN(token_t, data_type, elem)                              \
    tokens->array_tokens[tokens->size].type = token_t;                   \
    tokens->array_tokens[tokens->size].data.data_type = elem;            \
    tokens->size += 1;                                                   \
    ptr++;

#define PRINT_GR_LEAF(spec, elem)                                                                                                                    \
    fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = YellowGreen, label = \"" spec "\" ];\n", *node_num, node->data.elem)     \

#define PRINT_GR(spec, elem)                                                                                                                    \
    fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"" spec "\" ];\n", *node_num, node->data.elem)       \

#define PRINT_GR_SIGN(spec, elem)                                                                                                               \
    fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = \"" spec "\" ];\n", *node_num, elem)                  \


#endif // DSL_H_INCLUDED
