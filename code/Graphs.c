#include <stdio.h>
#include <stdlib.h>
#include "Graphs.h"

graph *initialize_graph (
    int directed,
    int multiplied,
    int weighted)
{
    graph *g = malloc (sizeof (graph));

    g -> first_vertex = NULL;
    g -> nvertices = 0;
    g -> nedges = 0;
    g -> directed = directed;
    g -> multiplied = multiplied;
    g -> weighted = weighted;
    return g;
}

graph *insert_vertex_to_graph (
    graph *g,
    item_type item)
{
    vertex *tmp = g -> first_vertex;

    tmp = insert_vertex (tmp, item);
    g -> first_vertex = tmp;
    g -> nvertices = count_vertex (tmp);
    return g;
}

graph *insert_edge_to_graph (
    graph *g,
    item_type vert,
    item_type edge)
{
    vertex *tmp = g -> first_vertex;

    tmp = insert_edge_to_vertex (tmp, vert, edge, g -> multiplied, g -> directed);
    g -> first_vertex = tmp;
    g -> nedges = count_edges_in_vertex (tmp);
    return g;
}

graph *set_weight_of_edge_in_graph (
    graph *g,
    item_type vert,
    item_type edge,
    int weight)
{
    vertex *tmp = g -> first_vertex;

    tmp = set_weight_of_edge_in_vertex (tmp, vert, edge, weight, g -> directed, g -> weighted);
    g -> first_vertex = tmp;
    return g;
}

graph *del_vertex_from_graph (
    graph *g,
    item_type item)
{
    vertex *tmp = g -> first_vertex;

    tmp = del_one_vertex (tmp, item);
    g -> first_vertex = tmp;
    g -> nvertices = count_vertex (tmp);
    g -> nedges = count_edges_in_vertex (tmp);
    return g;
}

graph *del_edge_from_graph (
    graph *g,
    item_type vert,
    item_type edge)
{
    vertex *tmp = g -> first_vertex;

    tmp = del_edge_from_vertex(tmp, vert, edge, g -> multiplied, g -> directed);
    g -> first_vertex = tmp;
    g -> nedges = count_edges_in_vertex (tmp);
    return g;
}

graph *delete_graph (
    graph *g)
{
    vertex *tmp = g -> first_vertex;

    tmp = delete_vertex (tmp);
    g -> first_vertex = tmp;
    free (g);
    return NULL;
}

void print_graph (
    const graph *g)
{
    if (g == NULL) {
        printf ("\nGraph is not yet initialized!\n");
        return;
    }
    printf ("\nGraph ");
    if (!g -> nvertices) {
        printf ("is empty, ");
    } else {
        printf ("with ");
        if (g -> nvertices == 1) {
            printf ("1 vertex ");
        } else {
            printf ("%d vertices ", g -> nvertices);
        }
    }
    if (g -> nvertices) {
        if (!g -> nedges) {
            printf ("and no edges, ");
        } else if (g -> nedges == 1) {
            printf ("and 1 edge, ");
        } else {
            printf ("and %d edges, ", g -> nedges);
        }
    }
    if (g -> directed) {
        printf ("directed, ");
    } else {
        printf ("undirected, ");
    }
    if (g -> multiplied) {
        printf ("with multiple edges and ");
    } else {
        printf ("without multiple edges and ");
    }
    if (g -> weighted) {
        printf ("weighted");
    } else {
        printf ("unweighted");
    }
    if (!g -> nvertices) {
        printf ("!\n\n");
        return;
    } else {
        printf (", has:\n\n");
        print_vertex (g -> first_vertex, g -> multiplied, g -> weighted);
    }
}
