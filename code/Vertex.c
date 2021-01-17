#include <stdio.h>
#include <stdlib.h>
#include "Vertex.h"

vertex *search_vertex (
    const vertex *v,
    item_type item)
{
    return (v == NULL || v -> item == item)? (vertex*) v :
           search_vertex (v -> next_vertex, item);
}

vertex *insert_vertex (
    vertex *v,
    item_type item)
{
    vertex *tmp;
    if ((tmp = search_vertex (v, item)) != NULL) {
        DEBUG ("Error in insert_vertex(): this vertex is already in graph.\n");
        return v;
    } else {
        tmp = malloc (sizeof (vertex));
        tmp -> item = item;
        tmp -> degree = 0;
        tmp -> edges = NULL;
        tmp -> next_vertex = v;
        return tmp;
    }
}

vertex *insert_edge_to_vertex (
    vertex *v,
    item_type vert,
    item_type edge,
    int multi,
    int direct)
{
    vertex *tmp;

    if ((tmp = search_vertex(v, vert)) == NULL || search_vertex(v, edge) == NULL) {
        DEBUG ("Error in insert_edge_to_vertex(): there is not vertex of source or destinator.\n");
    } else {
        list *l = tmp -> edges;

        l = update_list (l, edge, multi);
        tmp -> edges = l;
        tmp -> degree++;
        if (!direct && vert != edge) {
            v = insert_edge_to_vertex (v, edge, vert, multi, !direct);
        }
    }
    return v;
}

vertex *set_weight_of_edge_in_vertex (
    vertex *v,
    item_type vert,
    item_type edge,
    int weight,
    int directed,
    int permission)
{
    if (!permission) {
        DEBUG ("Error in set_weight_of_edge_in_vertex(): graph is unweighted.\n");
    } else {
        vertex *tmp;

        if ((tmp = search_vertex(v, vert)) == NULL || search_vertex(v, edge) == NULL) {
            DEBUG ("Error in set_weight_of_edge_in_vertex(): there is not vertex of source or destinator.\n");
        } else {
            list *l = tmp -> edges;

            l = set_weight(l, edge, weight);
            tmp -> edges = l;
            if (!directed) {
                v = set_weight_of_edge_in_vertex(v, edge, vert, weight, !directed, permission);
            }
        }
    }
    return v;
}

int count_vertex (
    const vertex *v)
{
    return (v == NULL)? 0 : 1 + count_vertex (v -> next_vertex);
}

int count_edges_in_vertex (
    const vertex *v)
{
    return (v == NULL)? 0 : v -> degree + count_edges_in_vertex(v -> next_vertex);
}

static vertex* predecessor_vertex (
    const vertex *v,
    item_type item)
{
    if (v == NULL) {
        DEBUG ("Error in predecessor_vertex(): predecessor sought on null vertex.\n");
        return NULL;
    }
    return (v -> next_vertex == NULL)? NULL :
           ((v -> next_vertex) -> item == item)? (vertex*) v :
           predecessor_vertex (v -> next_vertex, item);
}

vertex *del_one_vertex (
    vertex *v,
    item_type item)
{
    vertex *tmp;
    vertex *pred;
    list *l_local;

    tmp = search_vertex (v, item);
    if (tmp != NULL) {
        pred = predecessor_vertex (v, item);
        if (pred == NULL) {
            v = tmp -> next_vertex;
        } else {
            pred -> next_vertex = tmp -> next_vertex;
        }
        delete_list (tmp -> edges);
        free (tmp);
        for (vertex *v_local = v; v_local != NULL; v_local = v_local -> next_vertex) {
            l_local = v_local -> edges;
            if (search_list (l_local, item) != NULL) {
                l_local = del_edge_from_list (l_local, item, 0);
                v_local -> degree = count_edges (l_local);
            }
            v_local -> edges = l_local;
        }
    } else {
        DEBUG ("Error in del_one_vertex(): there is not yet this vertex in graph.\n");
    }
    return v;
}

vertex *del_edge_from_vertex (
    vertex *v,
    item_type vert,
    item_type edge,
    int multi,
    int direct)
{
    vertex *tmp;

    if ((tmp = search_vertex (v, vert)) == NULL || search_vertex (v, edge) == NULL) {
        DEBUG ("Error in del_edge_from_vertex(): there is not vertex of source or destinator.\n");
    } else {
        list *l = tmp -> edges;

        l = del_edge_from_list (l, edge, multi);
        tmp -> edges = l;
        tmp -> degree = count_edges (l);
        if (!direct) {
            v = del_edge_from_vertex (v, edge, vert, multi, !direct);
        }
    }
    return v;
}

vertex *delete_vertex (
    vertex *v)
{
    if (v == NULL) {
        return NULL;
    }
    vertex *tmp = v;

    if (tmp -> next_vertex != NULL) {
        v = tmp -> next_vertex;
        delete_list (tmp -> edges);
        free (tmp);
        return delete_vertex (v);
    }
    delete_list (tmp -> edges);
    free (tmp);
    return NULL;
}

void print_vertex (
    const vertex *v,
    int multiplied,
    int weighted)
{
    if (v != NULL) {
        printf("  Vertex %d (degree %d)", v -> item, v -> degree);
        if (!v -> degree) {
            if (v -> next_vertex == NULL) {
                printf(".\n");
            } else {
                printf(";\n");
            }
        } else {
            printf(":\n");
        }
        print_list(v -> edges, multiplied, weighted);
        printf("\n");
        print_vertex(v -> next_vertex, multiplied, weighted);
    }
}
