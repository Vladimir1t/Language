#include "..\include\translator.h"

int main (int argc, char* argv[])
{
    //FOPEN (file_output, "text_files\\asm_file.txt", "w");
    struct Node* root = NULL;

    printf ("\n------- Translator -------\n\n");

    if (get_database (&root, argv[1]) != SUCCESS)
        return 0;

    const char file_graph[] = "Backend\\graphviz\\graph.dot";

    char choice = '\0';
    while (choice != 'n' && choice != 'y')
    {
        printf ("Do you want to print data_tree?\n"
                "( y, n )\n");
        scanf ("%c", &choice);
        if (choice == 'y')
        {
            build_graphviz (root, file_graph);
            system ("dot -Tpng Backend\\graphviz\\graph.dot -o Backend\\graphviz\\tree_graph.png");
            system ("start Backend\\graphviz\\tree_graph.png");
        }
        clean_buffer ();
    }
    /*simplifier (root);

    choice = '\0';
    while (choice != 'n' && choice != 'y')
    {
        printf ("Do you want to print data_tree?\n"
                "( y, n )\n");
        scanf ("%c", &choice);
        if (choice == 'y')
        {
            build_graphviz (root, file_graph);
            system ("dot -Tpng Frontend\\graphviz\\graph.dot -o Frontend\\graphviz\\tree_graph.png");
            system ("start Frontend\\graphviz\\tree_graph.png");
        }
        clean_buffer ();
    }
    tree_output (root, file_output);*/

    tree_dtor (root);

    return 0;
}

