#include <stdio.h>
#include <stdlib.h>
#include "Distances.h"

static list_of_distances *search_in_distances (
    const list_of_distances *lod,
    item_type item)
{
    return (lod == NULL || lod -> item == item)? (list_of_distances*) lod :
           search_in_distances (lod -> next_vertex, item);
}

static list_of_distances *update_distance (
    list_of_distances *lod,
    item_type item)
{
    list_of_distances *tmp;

    if ((tmp = search_in_distances(lod, item)) != NULL) {
        DEBUG ("Error in update_distance(): such vertex is already exists.\n");
        return lod;
    } else {
        tmp = malloc (sizeof (list_of_distances));
        tmp -> item = item;
        tmp -> business = 0;
        tmp -> distance = -1;
        tmp -> next_vertex = lod;
        return tmp;
    }
}

list_of_distances *delete_distances (
    list_of_distances *lod)
{
    if (lod == NULL) {
        return NULL;
    }
    list_of_distances *tmp = lod;
    if (tmp -> next_vertex != NULL) {
        lod = tmp -> next_vertex;
        free (tmp);
        return delete_distances (lod);
    } else {
        free (tmp);
        return NULL;
    }
}

list_of_distances *dijkstra (
    graph *g,
    item_type item)
{
    list_of_distances *distances = NULL;
    list_of_distances *tmp_dist;
    vertex *tmp_vert = g -> first_vertex;

    for (; tmp_vert != NULL; tmp_vert = tmp_vert -> next_vertex) {
        distances = update_distance (distances, tmp_vert -> item);
    }

    if ((tmp_dist = search_in_distances(distances, item)) == NULL) {
        DEBUG ("Error in dijkstra(): such vertex is not found.\n");
    } else {
        tmp_dist -> distance = 0;
        tmp_dist -> business = 1;

        int min_dist;
        item_type item_dist;

        do {
            min_dist = -1;
            for (tmp_vert = g -> first_vertex; tmp_vert != NULL; tmp_vert = tmp_vert -> next_vertex) {
                tmp_dist = search_in_distances(distances, tmp_vert -> item);
                if (!tmp_dist -> business) {
                    continue;
                } else {
                    for (list *tmp_list = tmp_vert -> edges; tmp_list != NULL; tmp_list = tmp_list -> next) {
                        if (search_in_distances(distances, tmp_list -> item) -> business) {
                            continue;
                        }
                        int local_dist = tmp_list -> weight + tmp_dist -> distance;
                        if (local_dist < min_dist || min_dist == -1) {
                            min_dist = local_dist;
                            item_dist = tmp_list -> item;
                        }
                    }
                }
            }
            if (min_dist != -1) {
                tmp_dist = search_in_distances (distances, item_dist);
                tmp_dist -> business = 1;
                tmp_dist -> distance = min_dist;
            }
        } while (min_dist != -1);
    }
    return distances;
}

void print_distances (
    const list_of_distances *lod)
{
    list_of_distances *tmp_dist = (list_of_distances*) lod;

    printf("\n");
    for (; tmp_dist != NULL; tmp_dist = tmp_dist -> next_vertex) {
        if (tmp_dist -> business) {
            printf("Distance to the vertex %d is %d\n", tmp_dist -> item, tmp_dist -> distance);
        } else {
            printf("Vertex %d is unachivable!\n", tmp_dist -> item);
        }
    }
    printf("\n");
}
