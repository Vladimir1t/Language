#include "..\include\translator.h"

int run_translator (struct Node* root, char* asm_file)
{
    CHECK_PTR (root);
    CHECK_PTR (asm_file);

    FOPEN (asm_file_p, asm_file, "w");

    struct Variables variables = {0};
    make_asm_code (root, &variables);

}

int make_asm_code (struct Node* root, struct Variables)
