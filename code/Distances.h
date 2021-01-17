#pragma once

#include "Graphs.h"

typedef struct list_of_busy_vertices {
    item_type item;
    int business;
    int distance;
    struct list_of_busy_vertices *next_vertex;
} list_of_distances;

list_of_distances *delete_distances (list_of_distances*);
list_of_distances *dijkstra (graph*, item_type);
void print_distances (const list_of_distances*);
