#pragma once

#include "Lists.h"

typedef struct vertex {
    int degree;
    item_type item;
    list *edges;
    struct vertex *next_vertex;
} vertex;

vertex *search_vertex (const vertex*, item_type);
vertex *insert_vertex (vertex*, item_type);
vertex *insert_edge_to_vertex (vertex*, item_type, item_type, int, int);
vertex *set_weight_of_edge_in_vertex (vertex*, item_type, item_type, int, int, int);
int count_vertex (const vertex*);
int count_edges_in_vertex (const vertex*);
vertex *del_one_vertex (vertex*, item_type);
vertex *del_edge_from_vertex (vertex*, item_type, item_type, int, int);
vertex *delete_vertex (vertex*);
void print_vertex (const vertex*, int, int);
