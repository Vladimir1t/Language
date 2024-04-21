#include "..\include\parcer.h"


int main (int argc, char* argv[])
{
    FOPEN (file_output, "file_output.txt", "w");
    struct Node* root = NULL;
    printf ("\n-- LANGUAGE --\n\n");
    if (get_database (&root, argv[1]) != SUCCESS)
        return 0;

    const char file_graph[] = "graphviz\\graph.dot";

    char choice = '\0';
    while (choice != 'n' && choice != 'y')
    {
        printf ("Do you want to print data_tree?\n"
                "( y, n )\n");
        scanf ("%c", &choice);
        if (choice == 'y')
        {
            build_graphviz (root, file_graph);
            system ("dot -Tpng graphviz\\graph.dot -o graphviz\\tree_graph.png");
            system ("start graphviz\\tree_graph.png");
        }
        clean_buffer ();
    }
    simplifier (root);

    choice = '\0';
    while (choice != 'n' && choice != 'y')
    {
        printf ("Do you want to print data_tree?\n"
                "( y, n )\n");
        scanf ("%c", &choice);
        if (choice == 'y')
        {
            build_graphviz (root, file_graph);
            system ("dot -Tpng graphviz\\graph.dot -o graphviz\\tree_graph.png");
            system ("start graphviz\\tree_graph.png");
        }
        clean_buffer ();
    }

    //simplifier (root);

    //struct Node* diff_tree = run_differentiator (root, file_output);

    tree_dtor (root);

    return 0;
}
