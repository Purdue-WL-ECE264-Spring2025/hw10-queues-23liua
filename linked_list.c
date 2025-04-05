#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

struct list_node *create_node(size_t val) {
    struct list_node *node = malloc(sizeof(struct list_node));
    if (node) {
        node->value = val;
        node->next = NULL;
    }
    return node;
}

void insert_at_head(struct linked_list *lst, size_t num) {
    struct list_node *new = create_node(num);
    if (!new) return;
    
    new->next = lst->head;
    lst->head = new;
}

void append_at_tail(struct linked_list *lst, size_t val) {
    struct list_node *node = create_node(val);
    if (!node) return;

    if (!lst->head) {
        lst->head = node;
        return;
    }

    struct list_node *current = lst->head;
    while (current->next) {
        current = current->next;
    }
    current->next = node;
}

size_t pop_from_head(struct linked_list *lst) {
    if (!lst->head) return 0;
    
    struct list_node *temp = lst->head;
    size_t value = temp->value;
    lst->head = temp->next;
    free(temp);
    
    return value;
}

size_t remove_last(struct linked_list *lst) {
    if (!lst->head) return 0;

    struct list_node *current = lst->head;
    struct list_node *previous = NULL;

    while (current->next) {
        previous = current;
        current = current->next;
    }

    size_t val = current->value;
    if (!previous) {
        lst->head = NULL;
    } else {
        previous->next = NULL;
    }
    free(current);
    
    return val;
}

void destroy_list(struct linked_list *list) {
    struct list_node *current = list->head;
    while (current) {
        struct list_node *next = current->next;
        free(current);
        current = next;
    }
    list->head = NULL;
}

void display_list(FILE *fp, struct linked_list *list) {
    fprintf(fp, "{ ");
    for (struct list_node *node = list->head; node; node = node->next) {
        fprintf(fp, "%zu ", node->value);
    }
    fprintf(fp, "}\n");
}
