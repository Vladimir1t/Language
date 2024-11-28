#include "backend.h"

int main (int argc, char* argv[])
{
    struct Node* root = NULL;

    printf ("\n---------- LANGUAGE Translator ----------\n\n");
    
    if (get_database (&root, argv[1]) != SUCCESS)
        return 0;

    const char file_graph[] = "Backend/graphviz/graph.dot";

    #ifndef NDEBUG
        char choice = '\0';
        while (choice != 'n' && choice != 'y')
        {
            printf ("- Do you want to print data_tree?\n"
                    "( y, n )\n");
            scanf ("%c", &choice);
            if (choice == 'y')
            {
                build_graphviz (root, file_graph);
                system ("dot -Tpng Backend/graphviz/graph.dot -o Backend/graphviz/tree_graph.png");
                system ("start Backend/graphviz/tree_graph.png");
            }
            clean_buffer ();
        }
    #endif

    run_translator (root, argv[2]);

    tree_dtor (root);

    return 0;
}

