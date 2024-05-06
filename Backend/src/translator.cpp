#include "..\include\backend.h"

static FILE* error_file = fopen ("log\\error_file_translator.txt", "w");

static int make_asm_code      (struct Node* node, struct Variables* variables, FILE* asm_file);

static int add_str_end_asm    (struct Node* node, FILE* asm_file, struct Variables* variables);
static int add_assignment_asm (struct Node* node, FILE* asm_file, struct Variables* variables);
static int add_if_asm         (struct Node* node, FILE* asm_file, struct Variables* variables);
static int add_while_asm      (struct Node* node, FILE* asm_file, struct Variables* variables);
static int add_func_asm       (struct Node* node, FILE* asm_file, struct Variables* variables);

static int define_sign    (char* sign);
static int define_op_long (char* op);

static int label_num = 10;
static int err       = 0;

int run_translator (struct Node* root, char* output_file)
{
    CHECK_PTR (root);
    CHECK_PTR (output_file);

    FOPEN (asm_file, output_file, "w");

    struct Variables variables = {0};   // array of variables in program and their addresses in RAM
    variables.capacity = 5;
    CALLOC (variables.array_var, struct Variable,  variables.capacity);

    make_asm_code (root, &variables, asm_file);
    fprintf (asm_file, "hlt\n");

    fclose (asm_file);
}

int make_asm_code (struct Node* node, struct Variables* variables, FILE* asm_file)
{
    CHECK_PTR (node);
    CHECK_PTR (variables);
    CHECK_PTR (asm_file);

    if (err == 1)
        return ERROR;
    printf ("-------------------------------\n");
    printf ("type [%d]\n", node->type);

    if (node->type == T_NUM)
    {
        fprintf (asm_file, "push %d\n", node->data.value);
    }
    else if (node->type == T_VAR)
    {
        ADD_VAR_ASM (asm_file, Variables);
    }
    else if (node->type == T_KEY_W && node->data.key_w == '=')
    {
        add_assignment_asm (node, asm_file, variables);
    }
    else if (node->type == T_KEY_W && node->data.key_w == ';')
    {
        add_str_end_asm (node, asm_file, variables);
    }
    else if (node->type == T_OP)
    {
        switch (node->data.op)
        {
            case '+':
                ADD_OP_ASM ("add\n")
                break;
            case '-':
                ADD_OP_ASM ("sub\n");
                break;
            case '*':
                ADD_OP_ASM ("mul\n");
                break;
            case '/':
                ADD_OP_ASM ("div\n");
                break;
            case '^':
                ADD_OP_ASM ("pow\n");
                break;
        }
    }
    else if (node->type == T_OP_LONG)
    {
        switch (define_op_long (node->data.op_long))
        {
            case OP_SIN:
                ADD_OP_LONG_ASM ("sin\n");
                break;
            case OP_COS:
                ADD_OP_LONG_ASM ("cos\n");
                break;
            case OP_EXP:
                ADD_OP_LONG_ASM ("exp\n");
                break;
            case OP_LN:
                ADD_OP_LONG_ASM ("ln\n");
                break;
            case OP_SQRT:
                ADD_OP_LONG_ASM ("sqrt\n");
                break;
            case OP_TG:
                ADD_OP_LONG_ASM ("tg\n");
                break;
            case OP_CTG:
                ADD_OP_LONG_ASM ("ctg\n");
                break;
        }
    }
    else if (node->type == T_IF_)
    {
        add_if_asm (node, asm_file, variables);
    }
    else if (node->type == T_WHILE)
    {
        add_while_asm (node, asm_file, variables);
    }
    else if (node->type == T_FUNC)
    {
        add_func_asm (node, asm_file, variables);
    }
    else
        return ERROR;

    return SUCCESS;
}

int add_str_end_asm (struct Node* node, FILE* asm_file, struct Variables* variables)
{
    //printf (";\n");
    if (node->left != NULL && node->left->type != DEFUALT)
        make_asm_code (node->left, variables, asm_file);

    if (node->right != NULL && node->right->type != DEFUALT)
        make_asm_code (node->right, variables, asm_file);
}

int add_assignment_asm (struct Node* node, FILE* asm_file, struct Variables* variables)
{
    //printf ("[=]\n");
    make_asm_code (node->right, variables, asm_file);
    if (node->left->type == T_VAR)
    {
        int flag = 0;
        for (int i = 0; i < variables->size; i++)
            if (!strcmp (node->left->data.var, variables->array_var[i].name))
            {
                flag = 1;
                fprintf (asm_file, "pop [%d]\n", variables->array_var[i].adr);
            }
        if (flag == 0)
        {
            if (variables->size == variables->capacity)
            {
                INCREASE_CAPACITY (variables);
            }
            strcpy (variables->array_var[variables->size].name, node->left->data.var);
            variables->array_var[variables->size].adr = variables->size;
            fprintf (asm_file, "pop [%d]\n", variables->array_var[variables->size].adr);
            variables->size += 1;
        }
    }
    else
        return ERROR;
}

int add_if_asm (struct Node* node, FILE* asm_file, struct Variables* variables)
{
    //printf ("if\n");
    if (node->left != NULL && node->left->type == T_SIGN)
    {
        printf ("[%s]\n", node->left->data.sign);
        if (node->left->left != NULL && node->left->left->type != DEFUALT)
            make_asm_code (node->left->left, variables, asm_file);
        if (node->left->right != NULL && node->left->right->type != DEFUALT)
            make_asm_code (node->left->right, variables, asm_file);
        switch (define_sign (node->left->data.sign))
        {
            case B:
                fprintf (asm_file, "jae lb_%d:  # if\n", label_num);
                break;
            case A:
                fprintf (asm_file, "jbe lb_%d:  # if\n", label_num);
                break;
            case E:
                fprintf (asm_file, "jne lb_%d:  # if\n", label_num);
                break;
            case BE:
                fprintf (asm_file, "ja lb_%d:  # if\n", label_num);
                break;
            case AE:
                fprintf (asm_file, "jb lb_%d:  # if\n", label_num);
                break;
            case NE:
                fprintf (asm_file, "je lb_%d:  # if\n", label_num);
                break;
        }
        int label = label_num++;
        if (node->right != NULL && node->right->type != DEFUALT)
            make_asm_code (node->right, variables, asm_file);
        fprintf (asm_file, "lb_%d:\n", label);
    }
    else
        return ERROR;
}

int add_while_asm (struct Node* node, FILE* asm_file, struct Variables* variables)
{
    //printf ("if\n");
    if (node->left != NULL && node->left->type == T_SIGN)
    {
        printf ("[%s]\n", node->left->data.sign);

        int first_label = label_num++;
        fprintf (asm_file, "lb_%d:   # while\n", first_label);

        if (node->left->left != NULL && node->left->left->type != DEFUALT)
            make_asm_code (node->left->left, variables, asm_file);
        if (node->left->right != NULL && node->left->right->type != DEFUALT)
            make_asm_code (node->left->right, variables, asm_file);


        switch (define_sign (node->left->data.sign))
        {
            case B:
                fprintf (asm_file, "jae lb_%d:\n", label_num);
                break;
            case A:
                fprintf (asm_file, "jbe lb_%d:\n", label_num);
                break;
            case E:
                fprintf (asm_file, "jne lb_%d:\n", label_num);
                break;
            case BE:
                fprintf (asm_file, "ja lb_%d:\n", label_num);
                break;
            case AE:
                fprintf (asm_file, "jb lb_%d:\n", label_num);
                break;
            case NE:
                fprintf (asm_file, "je lb_%d:\n", label_num);
                break;
        }
        int label = label_num++;
        if (node->right != NULL && node->right->type != DEFUALT)
            make_asm_code (node->right, variables, asm_file);

        fprintf (asm_file, "jmp lb_%d:  # jump to start of while\n", first_label);

        fprintf (asm_file, "lb_%d:\n", label);
    }
    else
        return ERROR;
}

int add_func_asm (struct Node* node, FILE* asm_file, struct Variables* variables)
{
    if (!strcmp (node->data.func, "in"))
    {
        fprintf (asm_file, "in\n");
        if (node->left->type == T_VAR)
        {
            int flag = 0;
            for (int i = 0; i < variables->size; i++)
                if (!strcmp (node->left->data.var, variables->array_var[i].name))
                {
                    flag = 1;
                    fprintf (asm_file, "pop [%d]\n", variables->array_var[i].adr);
                }
            if (flag == 0)
            {
                if (variables->size == variables->capacity)
                {
                    INCREASE_CAPACITY (variables);
                }
                strcpy (variables->array_var[variables->size].name, node->left->data.var);
                variables->array_var[variables->size].adr = variables->size;
                fprintf (asm_file, "pop [%d]\n", variables->array_var[variables->size].adr);
                variables->size += 1;
            }
        }
        else
            return ERROR;
    }
    if (!strcmp (node->data.func, "out"))
    {
        if (node->left != NULL && node->left->type != DEFUALT)
            make_asm_code (node->left, variables, asm_file);
        fprintf (asm_file, "out\n");
    }
}

int define_sign (char* sign)
{
    CHECK_PTR (sign);

    if (!strcmp (sign, "<"))
        return B;
    else if (!strcmp (sign, ">"))
        return A;
    else if (!strcmp (sign, "=="))
        return E;
    else if (!strcmp (sign, "<="))
        return BE;
    else if (!strcmp (sign, ">="))
        return AE;
    else if (!strcmp (sign, "!="))
        return NE;
}

int define_op_long (char* op)
{
    CHECK_PTR (op);

    if (!strcmp (op, "cos"))
        return OP_COS;
    else if (!strcmp (op, "sin"))
        return OP_SIN;
    else if (!strcmp (op, "sin"))
        return OP_SIN;
    else if (!strcmp (op, "ln"))
        return OP_LN;
    else if (!strcmp (op, "exp"))
        return OP_EXP;
    else if (!strcmp (op, "sqrt"))
        return OP_SQRT;
    else if (!strcmp (op, "tg"))
        return OP_TG;
    else if (!strcmp (op, "ctg"))
        return OP_CTG;
}
