#include "..\include\ .h"

static FILE* file_error = fopen ("Backend\\log\\file_error_input_output.txt", "w");

static int add_node_in_graph_1 (struct node_tree* node, FILE* file_graph, size_t* node_num);

static int add_node_in_graph_2 (struct node_tree* node, FILE* file_graph);

int get_database (struct node_tree** root, char* file_input)   // get data of tree in the following file
{
    CALLOC (*root, struct node_tree, 1);
    CALLOC ((*root)->data, char, DATA_SIZE);

    FOPEN (file_p, file_input, "rb");
    size_t file_size = file_size_measure (file_p);            // measures the size of a text
    //printf ("size of file: %d\n", file_size);

    char* text_data = NULL;
    CALLOC (text_data, char, file_size + 1);

    size_t factual_size = fread (text_data, sizeof (char), file_size, file_p);
    if (factual_size != file_size)
    {
        fprintf (file_error, "<< Error. Wrong size of data_file >>\n");
        return ERROR;
    }
    fclose (file_p);

    construct_data_nodes (*root, text_data, file_size);

    return SUCCESS;
}

int construct_data_nodes (struct node_tree* root, char* text_data, size_t file_size)
{
    struct node_tree* prev_node = NULL;
    struct stack      stk       = {0};

    printf (" DATA_BASE \n");
    stack_ctor (&stk, 2);
    for (int i = 0, n = 0, position = ROOT; i < file_size; i++)
    {
        if (text_data[i] == '{' && position == ROOT)  // add a root of the tree
        {
            while (text_data[i] != '"')               // search for a beginning of a new word
                 i++;
            i++;

            while (text_data[i] != '"')               // fill buffer with a new word
            {
                root->data[n] = text_data[i];
                i++;
                n++;
            }
            root->data[n] = '\0';
            printf ("<< %s >>\n", root->data);

            stack_push (&stk, &root);
            stack_push (&stk, &root);
            position = LEFT;
        }
        if (text_data[i] == '{')                      // add node in tree
        {
            struct node_tree* node = NULL;
            CALLOC (node, struct node_tree, 1);
            CALLOC (node->data, char, DATA_SIZE);
            n = 0;
            while (text_data[i] != '"')               // search for a beginning of a new word
                i++;
            i++;

            while (text_data[i] != '"')               // fill buffer with a new word
            {
                node->data[n++] = text_data[i++];
            }
            node->data[n] = '\0';
            printf ("<< %s >>\n", node->data);
            stack_pop (&stk, &prev_node);

            if (position == LEFT)                     // add node as a left leaf
                prev_node->left = node;
            else                                      // add node as a right leaf
                prev_node->right = node;

            while (text_data[i] != '{' && text_data[i] != '}')
            {
                i++;
            }
            if (text_data[i] == '{')                  // next node will be in a left position
            {
                stack_push (&stk, &node);
                stack_push (&stk, &node);
                position = LEFT;
            }
            i--;
        }
        else if (text_data[i] == '}')                 // if a next node will be, it will stay in a right position
        {
            position = RIGHT;
        }
    }
    printf ("\n");
    stack_dtor (&stk);

    return SUCCESS;
}

size_t file_size_measure (FILE* const file_p)
{
    assert (file_p != NULL);

    int start_position = ftell (file_p);
    fseek (file_p, 0, SEEK_END);
    size_t len = (size_t) ftell (file_p);

    fseek (file_p, start_position, SEEK_SET);

    return len;
}

int build_graphviz (struct node_tree* root)
{
    size_t node_num = 0;
    if (root == NULL)
        return ERROR;

    FOPEN (file_graph, "graphviz\\graph.dot", "w");

    fprintf (file_graph, "digraph AKINATOR{\n"
                         "label = < AKINATOR >;\n"
                         "bgcolor = \"#BAF0EC\";\n"
                         "node [shape = record ];\n"
                         "edge [style = filled ];\n");

        /*   create a tree in graphviz   */
    add_node_in_graph_1 (root, file_graph, &node_num);
    add_node_in_graph_2 (root, file_graph);
    fprintf (file_graph, "}");

    fclose (file_graph);

    return SUCCESS;
}

static int add_node_in_graph_1 (struct node_tree* node, FILE* file_graph, size_t* node_num)
{

    char* buffer = (char*) calloc (strlen (node->data) + 3, sizeof (char));
    buffer[0] = '"';
    strcat (buffer, node->data);
    buffer[strlen (node->data) + 1] = '"';

    if (node->right == NULL && node->left == NULL)
    {
        fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = YellowGreen, label = %s ];\n", *node_num, buffer);
        node->num_in_tree = *node_num;
        //printf ("[%d]\n", node->num_in_tree);
    }
    else
    {
        fprintf (file_graph, " %d [shape = Mrecord, style = filled, fillcolor = Peru, label = %s ];\n", *node_num, buffer);
        node->num_in_tree = *node_num;
        //printf ("[%d]\n", node->num_in_tree);
    }
    if (node->left != NULL)
    {
        *node_num += 1;
        add_node_in_graph_1 (node->left, file_graph, node_num);
    }

    if (node->right != NULL)
    {
        *node_num += 1;
        add_node_in_graph_1 (node->right, file_graph, node_num);
    }

    return SUCCESS;
}

static int add_node_in_graph_2 (struct node_tree* node, FILE* file_graph)
{
    if (node->left != NULL)
    {
        fprintf (file_graph, "%d -> %d[label = \" no\"] [ color = Peru ];\n", node->num_in_tree, (node->left)->num_in_tree);
        add_node_in_graph_2 (node->left, file_graph);
    }

    if (node->right != NULL)
    {
        fprintf (file_graph, "%d -> %d[label = \" yes\"] [ color = Peru ];\n", node->num_in_tree, (node->right)->num_in_tree);
        add_node_in_graph_2 (node->right, file_graph);
    }

    return SUCCESS;
}
