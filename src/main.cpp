#include "..\Frontend\include\parcer.h"

int main (int argc, char* argv[])
{
    FOPEN (file_output, "text_files\\file_output.txt", "w");
    struct Node* root = NULL;

    printf ("\n------- LANGUAGE -------\n\n");

    run_parcer (&root, argv[1]);



    // BACKEND



    tree_dtor (root);

    return 0;
}

