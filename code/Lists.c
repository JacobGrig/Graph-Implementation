#include <stdio.h>
#include <stdlib.h>
#include "Lists.h"

list *search_list (
    const list *l,
    item_type item)
{
    return (l == NULL || l -> item == item)? (list*) l :
           search_list (l -> next, item);
}

list *update_list (
    list *l,
    item_type item,
    int multi)
{
    list *tmp;

    if ((tmp = search_list (l, item)) != NULL) {
        if (multi) {
            tmp -> fold++;
        } else {
            DEBUG ("Error in update_list(): it is not multigraph and already has this edge.\n");
        }
        return l;
    } else {
        tmp = malloc (sizeof (list));
        tmp -> item = item;
        tmp -> next = l;
        tmp -> fold = 1;
        tmp -> weight = 1;
        return tmp;
    }
}

list *set_weight (
    list *l,
    item_type item,
    int weight)
{
    list *tmp;

    if ((tmp = search_list(l, item)) != NULL) {
        tmp -> weight = weight;
    } else {
        DEBUG ("Error in set_weight(): there is not this edge.\n");
    }
    return l;
}

int count_edges (
    const list *l)
{
    return (l == NULL)? 0 : l -> fold + count_edges (l -> next);
}

static list *predecessor_list (
    const list *l,
    item_type item)
{
    if (l == NULL) {
        DEBUG ("Error in predecessor_list(): predecessor sought on null list.\n");
        return NULL;
    }
    return (l -> next == NULL)? NULL :
           ((l -> next) -> item == item)? (list*) l :
           predecessor_list (l -> next, item);
}

list *del_edge_from_list (
    list *l,
    item_type item,
    int multi)
{
    list *tmp;
    list *pred;

    if ((tmp = search_list (l, item)) != NULL) {
        if (multi && tmp -> fold > 1) {
            tmp -> fold--;
        } else {
            pred = predecessor_list (l, item);
            if (pred == NULL) {
                l = tmp -> next;
            } else {
                pred -> next = tmp -> next;
            }
            free(tmp);
        }
    } else {
        DEBUG ("Error in del_item_from_list(): there is not such edge in list\n");
    }
    return l;
}

list *delete_list (
    list *l)
{
    if (l == NULL) {
        return NULL;
    }
    list *tmp = l;
    if (tmp -> next != NULL) {
        l = tmp -> next;
        free (tmp);
        return delete_list (l);
    } else {
        free (tmp);
        return NULL;
    }
}

void print_list (
    const list *l,
    int multiplied,
    int weighted)
{
    for (; l; l = l -> next) {
        printf ("\t- edge with key %d", l -> item);
        if (multiplied) {
            printf(", fold %d", l -> fold);
        }
        if (weighted) {
            printf(", weight %d", l -> weight);
        }
        printf("\n");
    }
}
