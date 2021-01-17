#pragma once

#include "Vertex.h"

typedef struct header_of_graph {
    int nvertices;
    int nedges;
    int directed;
    int multiplied;
    int weighted;
    struct vertex *first_vertex;
} graph;

graph *initialize_graph (int, int, int);
graph *insert_vertex_to_graph (graph*, item_type);
graph *insert_edge_to_graph (graph*, item_type, item_type);
graph *set_weight_of_edge_in_graph (graph*, item_type, item_type, int);
graph *del_vertex_from_graph (graph*, item_type);
graph *del_edge_from_graph (graph*, item_type, item_type);
graph *delete_graph (graph*);
void print_graph (const graph*);
