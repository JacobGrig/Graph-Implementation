#include <stdio.h>
#include <stdlib.h>
#include "Interface.h"

#define MAXSTR 10000

static int correctness_of_operation (
    int number,
    int operation,
    int cond,
    const char *str)
{
    if (str == NULL) {
        printf("Error in correctness_of_operation(): str sought on null string");
        return 0;
    }
    return !(cond != 2 ||
             *str != '\n' ||
             operation < 1 ||
             operation > number);
}

static char *get_own_line (
    char *string)
{
    int i = 0, c;
    for (c = getchar(); c != '\n' && c != EOF; c = getchar()) {
        string [i++] = c;
    }
    string [i++] = '\n';
    string [i] = '\0';
    return string;
}

static void help_with_interface (
    const char *begin_string,
    const char *repeat_string,
    int *operation,
    int number_of_items)
{
    char *str = malloc (2 * sizeof(char) * MAXSTR);
    char *str_local = str + sizeof(char) * MAXSTR;
    int correctness, cond;

    printf ("%s", begin_string);
    str_local = get_own_line(str_local);
    cond = sscanf(str_local, "%d%c", operation, str);
    correctness = correctness_of_operation (number_of_items, *operation, cond, str);
    while (correctness == 0) {
        printf ("%s", repeat_string);
        str_local = get_own_line (str_local);
        cond = sscanf (str_local, "%d%c", operation, str);
        correctness = correctness_of_operation (number_of_items, *operation, cond, str);
    }
    free (str);
}

static int correctness_of_item (
    item_type item,
    int cond,
    const char *str)
{
    if (str == NULL ) {
        printf("Error in correctness_of_item(): str sought on null string");
        return 0;
    }
    return !(cond != 2 ||
             *str != '\n' ||
             item < 0);
}

static void read_vertex_edge_or_weight (
    const char *begin_string,
    const char *repeat_string,
    item_type *item)
{
    char *str = malloc (2 * sizeof(char) * MAXSTR);
    char *str_local = str + sizeof(char) * MAXSTR;
    int correctness, cond;

    printf ("%s", begin_string);
    str_local = get_own_line(str_local);
    cond = sscanf(str_local, "%d%c", item, str);
    correctness = correctness_of_item (*item, cond, str);
    while (correctness == 0) {
        printf ("%s", repeat_string);
        str_local = get_own_line (str_local);
        cond = sscanf (str_local, "%d%c", item, str);
        correctness = correctness_of_item (*item, cond, str);
    }
    free (str);
}

static void dijkstra_call (
    graph *g,
    item_type item)
{
    list_of_distances *distances_bet_vertices = dijkstra (g, item);
    print_distances (distances_bet_vertices);
    distances_bet_vertices = delete_distances (distances_bet_vertices);
}

static void request_handler (
    graph *g)
{
    int operation;
    char *begin_string, *repeat_string;

    if (g -> weighted) {
        begin_string = "    1. Insert vertex\n"
                       "    2. Insert edge\n"
                       "    3. Set weight to edge (unlocked for you, 'cause your graph is weighted)\n"
                       "    4. Calculate the distance from a given vertex to all the other\n"
                       "    5. Delete vertex\n"
                       "    6. Delete edge\n"
                       "    7. Print graph\n"
                       "    8. Delete graph and exit the program\n"
                       "\n"
                       "    Entry: ";

        repeat_string = "\nOh, it seems that you entered a non-existent item... Please try again!\n\n"
                        "    1. Insert vertex\n"
                        "    2. Insert edge\n"
                        "    3. Set weight to edge (unlocked for you, 'cause your graph is weighted)\n"
                        "    4. Calculate the distance from a given vertex to all the other\n"
                        "    5. Delete vertex\n"
                        "    6. Delete edge\n"
                        "    7. Print graph\n"
                        "    8. Delete graph and exit the program\n"
                        "\n"
                        "    Entry: ";
    } else {
        begin_string = "    1. Insert vertex\n"
                       "    2. Insert edge\n"
                       "    3. Set weight to edge (locked for you, 'cause your graph is unweighted)\n"
                       "    4. Calculate the distance from a given vertex to all the other\n"
                       "    5. Delete vertex\n"
                       "    6. Delete edge\n"
                       "    7. Print graph\n"
                       "    8. Delete graph and exit the program\n"
                       "\n"
                       "    Entry: ";

        repeat_string = "\nOh, it seems that you entered a non-existent item... Please try again!\n\n"
                        "    1. Insert vertex\n"
                        "    2. Insert edge\n"
                        "    3. Set weight to edge (locked for you, 'cause your graph is unweighted)\n"
                        "    4. Calculate the distance from a given vertex to all the other\n"
                        "    5. Delete vertex\n"
                        "    6. Delete edge\n"
                        "    7. Print graph\n"
                        "    8. Delete graph and exit the program\n"
                        "\n"
                        "    Entry: ";
    }
    help_with_interface (begin_string, repeat_string, &operation, 8);

    item_type vert, edge;
    int weight;
    vertex *v;

    switch (operation) {
        case 1:
            begin_string = "\nEnter the key of vertex you wanna add in graph (non-negative integer): ";
            repeat_string = "\nOh, it seems that you entered something not like non-negative integer!\n"
                            "Try again please: enter the key of vertex you wanna add in graph: ";
            read_vertex_edge_or_weight (begin_string, repeat_string, &vert);
            if (search_vertex(g -> first_vertex, vert) != NULL) {
                printf("\n  Such a vertex already exists!\n\n");
            } else {
                g = insert_vertex_to_graph(g, vert);
                printf("\n  Done!\n\n");
            }
            request_handler (g);
            break;
        case 2:
            begin_string = "\nEnter the key of vertex-parent of edge you wanna add in graph (non-negative integer): ";
            repeat_string = "\nOh, it seems that you entered something not like non-negative integer!\n"
                            "Try again please: enter the key of vertex-parent of edge you wanna add in graph: ";
            read_vertex_edge_or_weight (begin_string, repeat_string, &vert);
            if ((v = search_vertex (g -> first_vertex, vert)) == NULL) {
                printf("\n  Vertex of parent is not found!\n\n");
            } else {
                begin_string = "\nAnd now it's turn to vertex-child of this edge. Enter its key: ";
                repeat_string = "\nOh, it seems that you entered something not like non-negative integer!\n"
                                "Try again please: enter the key of vertex-child of edge you wanna add in graph: ";
                read_vertex_edge_or_weight (begin_string, repeat_string, &edge);
                if (search_vertex (g -> first_vertex, edge) == NULL) {
                    printf("\n  Vertex of child is not found!\n\n");
                } else if (search_list(v -> edges, edge) != NULL && !g -> multiplied) {
                    printf("\n  Your graph has not multiple edges, and such an edge already exists! Be careful!\n\n");
                } else {
                    g = insert_edge_to_graph(g, vert, edge);
                    printf("\n  Done!\n\n");
                }
            }
            request_handler (g);
            break;
        case 3:
            if (g -> weighted) {
                begin_string = "\nEnter the key of vertex-parent of edge you wanna set weight of (non-negative integer): ";
                repeat_string = "\nOh, it seems that you entered something not like non-negative integer!\n"
                                "Try again please: enter the key of vertex-parent of edge you wanna set weight: ";
                read_vertex_edge_or_weight (begin_string, repeat_string, &vert);
                if ((v = search_vertex (g -> first_vertex, vert)) == NULL) {
                    printf("\n  Vertex of parent is not found!\n\n");
                } else {
                    begin_string = "\nAnd now it's turn to vertex-child of this edge. Enter its key: ";
                    repeat_string = "\nOh, it seems that you entered something not like non-negative integer!\n"
                                    "Try again please: enter the key of vertex-child of edge you wanna set weight of: ";
                    read_vertex_edge_or_weight (begin_string, repeat_string, &edge);
                    if (search_vertex (g -> first_vertex, edge) == NULL) {
                        printf("\n  Vertex of child is not found!\n\n");
                    } else if (search_list(v -> edges, edge) == NULL) {
                        printf("\n  Such an edge is not found!\n\n");
                    } else {
                        begin_string = "\nEnter the weight of edge (not your weight!): ";
                        repeat_string = "\nOh, it seems that you entered something not like non-negative integer!\n"
                                        "Try again please: enter the weight of edge: ";
                        read_vertex_edge_or_weight (begin_string, repeat_string, &weight);
                        g = set_weight_of_edge_in_graph(g, vert, edge, weight);
                        printf("\n  Done!\n\n");
                    }
                }

            } else {
                printf("\nI told you that you can not call a function of weight\n"
                       "settings because your graph is unweighted! Be careful!\n\n");
            }
            request_handler (g);
            break;
        case 4:
            begin_string = "\nEnter the key of vertex you wanna calculate the distance from (non-negative integer): ";
            repeat_string = "\nOh, it seems that you entered something not like non-negative integer!\n"
                            "Try again please: enter the key of vertex you wanna calc dist from: ";
            read_vertex_edge_or_weight (begin_string, repeat_string, &vert);
            if (search_vertex (g -> first_vertex, vert) == NULL) {
                printf("\n Such a vertex is not found!\n\n");
            } else {
                dijkstra_call (g, vert);
            }
            request_handler (g);
            break;
        case 5:
            begin_string = "\nEnter the key of vertex you wanna delete from graph (non-negative integer): ";
            repeat_string = "\nOh, it seems that you entered something not like non-negative integer!\n"
                            "Try again please: enter the key of vertex you wanna delete from graph: ";
            read_vertex_edge_or_weight (begin_string, repeat_string, &vert);
            if (search_vertex(g -> first_vertex, vert) == NULL) {
                printf("\n  Such a vertex is not found!\n\n");
            } else {
                g = del_vertex_from_graph(g, vert);
                printf("\n  Done!\n\n");
            }
            request_handler (g);
            break;
        case 6:
            begin_string = "\nEnter the key of vertex-parent of edge you wanna delete from graph (non-negative integer): ";
            repeat_string = "\nOh, it seems that you entered something not like non-negative integer!\n"
                            "Try again please: enter the key of vertex-parent of edge you wanna delete from graph: ";
            read_vertex_edge_or_weight (begin_string, repeat_string, &vert);
            if ((v = search_vertex (g -> first_vertex, vert)) == NULL) {
                printf("\n  Vertex of parent is not found!\n\n");
            } else {
                begin_string = "\nAnd now it's turn to vertex-child of this edge. Enter its key: ";
                repeat_string = "\nOh, it seems that you entered something not like non-negative integer!\n"
                                "Try again please: enter the key of vertex-child of edge you wanna delete from graph: ";
                read_vertex_edge_or_weight (begin_string, repeat_string, &edge);
                if (search_vertex (g -> first_vertex, edge) == NULL) {
                    printf("\n  Vertex of child is not found!\n\n");
                } else if (search_list(v -> edges, edge) == NULL) {
                    printf("\n  Such an edge is not found!\n\n");
                } else {
                    g = del_edge_from_graph(g, vert, edge);
                    printf("\n  Done!\n\n");
                }
            }
            request_handler (g);
            break;
        case 7:
            print_graph(g);
            request_handler (g);
            break;
        case 8:
            return;
            break;
        default:
            DEBUG ("Error in request_handler(): unknown operation.\n");
            break;
    }
}

static void exit_program (
    void)
{
    printf("\nHow sad that you're leaving us so soon... We are waiting for you eagerly\n"
           "the next time and wish you to get ready for the nearest exam session perfectly well!\n\n");
}

static void creating_graph (
    void)
{
    int operation;
    int directed, multiplied, weighted;

    char *begin_string = "\nSo, you chose to create a graph. This is the right decision,\n"
                         "it characterizes you as wise and sensitive human,\n"
                         "and rest assured that it's still very useful. Let's start!\n"
                         "\n"
                         "We choose some options for the best graph in your life.\n"
                         "\n"
                         "Do you want it to be directed?\n"
                         "\n"
                         "    1. Yes, I love all directed things on the planet!\n"
                         "    2. No, fu, arrows, I hate them, it's not for me...\n"
                         "\n"
                         "    Entry: ";

    char *repeat_string = "\nOh, it seems that you entered a non-existent item... Please try again!\n"
                          "\n"
                          "    1. Create directed graph, I am fan of One Direction!\n"
                          "    2. Only undirected! Arrows squeezed all the juice out of me...\n"
                          "\n"
                          "    Entry: ";

    help_with_interface (begin_string, repeat_string, &operation, 2);
    switch (operation) {
        case 1:
            directed = 1;
            break;
        case 2:
            directed = 0;
            break;
        default:
            DEBUG ("Error in creating_graph(): unknown operation in directed.\n");
            break;
    }

    begin_string = "\nA good choice! No, really good! As good as a selection of\n"
                   "Donald Trump on the next US presidential election!\n"
                   "We continue!\n"
                   "\n"
                   "What about multiplicity of graph's edges?\n"
                   "\n"
                   "    1. Yes, I am just a maniac of multiple edges, I dream about it at night!\n"
                   "    2. No, I do not like to count the number, I am generally humanist!\n"
                   "\n"
                   "    Entry: ";

    repeat_string = "\nOh, it seems that you entered a non-existent item... Please try again!\n"
                    "\n"
                    "    1. Create multigraph, it sounds sublime (though I do not know what it is)!\n"
                    "    2. Without multi! I can not count money in the store, not to mention the edges...\n"
                    "\n"
                    "    Entry: ";

    help_with_interface (begin_string, repeat_string, &operation, 2);
    switch (operation) {
        case 1:
            multiplied = 1;
            break;
        case 2:
            multiplied = 0;
            break;
        default:
            DEBUG ("Error in creating_graph(): unknown operation in multiplied.\n");
            break;
    }

    begin_string = "\nOK, well-done! Whatever it was, you just do not take the army.\n"
                   "But who knows, who knows... Go to the next setting!\n"
                   "\n"
                   "Should your future graph be weighted?\n"
                   "\n"
                   "    1. Yes, if I have excess weight, why do my graph should not have?\n"
                   "    2. No, weight of the edge, are you serious? Tolerance crossed the line...\n"
                   "\n"
                   "    Entry: ";

    repeat_string = "\nOh, it seems that you entered a non-existent item... Please try again!\n"
                    "\n"
                    "    1. Create weighted, I love big, I love dangerous...!\n"
                    "    2. Unweighted, like in space! I love it, I imagine fried eggs \n"
                    "          for breakfast in the form of the sun!\n"
                    "\n"
                    "    Entry: ";

    help_with_interface (begin_string, repeat_string, &operation, 2);
    switch (operation) {
        case 1:
            weighted = 1;
            break;
        case 2:
            weighted = 0;
            break;
        default:
            DEBUG ("Error in creating_graph(): unknown operation in weighted.\n");
            break;
    }
    graph *g = initialize_graph (directed, multiplied, weighted);

    printf("\nThus, the primary tuning has passed, I congratulate you, my dear friend!\n"
           "Now you can select any of the following feature list.\n"
           "Remember that you can not create an edge between vertices that do not yet exist!\n\n");
    request_handler (g);
    g = delete_graph (g);
    begin_string = "\nYou were having fun with me, right? Of course right, because, according to\n"
                   "British scientists, manipulations with graphs help to relax from everyday worries,\n"
                   "stabilize the state of the body and enhance immunity!\n"
                   "\n"
                   "May be, again? Experiment with different settings!\n"
                   "\n"
                   "    1. Yes, I can not stop with it, more graphs, more!\n"
                   "    2. No, I certainly enjoyed it, but now I have to pour my ficus on the windowsill...\n"
                   "\n"
                   "    Entry: ";

    repeat_string = "\nOh, it seems that you entered a non-existent item... Please try again!\n"
                    "\n"
                    "    1. Create new graph, just create.\n"
                    "    2. Do not create new graph, just do not create.\n"
                    "\n"
                    "    Entry: ";

    help_with_interface (begin_string, repeat_string, &operation, 2);
    switch (operation) {
        case 1:
            creating_graph();
            break;
        case 2:
            exit_program();
    }
    return;
}

void start_graph (
    void)
{
    int operation;

    char *begin_string = "\nHello, my dear! Do you want to create a graph?\n"
                         "\n"
                         "Choose one of the next items:\n"
                         "\n"
                         "    1. Yes, of course! Create the best graph for me, please!\n"
                         "    2. No, why do I need all this? Exit the program and do not touch me anymore...\n"
                         "\n"
                         "    Entry: ";

    char *repeat_string = "\nOh, it seems that you entered a non-existent item... Please try again!\n"
                          "\n"
                          "    1. Create the best graph for me, please!\n"
                          "    2. Exit the program and do not touch me anymore...\n"
                          "\n"
                          "    Entry: ";

    help_with_interface (begin_string, repeat_string, &operation, 2);

    switch (operation) {
        case 1:
            creating_graph();
            break;
        case 2:
            exit_program();
            break;
        default:
            DEBUG ("Error in starter(): unknown operation.\n");
            break;
    }
}
