#pragma once

#define DEBUG(a) printf(a)

typedef int item_type;

typedef struct list_of_edges {
    int weight;
    int fold;
    item_type item;
    struct list_of_edges *next;
} list;

list *search_list (const list*, item_type);
list *update_list (list*, item_type, int);
list *set_weight (list*, item_type, int);
int count_edges (const list*);
list *del_edge_from_list (list*, item_type, int);
list *delete_list (list*);
void print_list (const list*, int, int);
