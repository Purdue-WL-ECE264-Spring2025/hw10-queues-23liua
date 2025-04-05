#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h> 


typedef struct custom_node {
    size_t value;
    struct custom_node *next;
    unsigned char metadata; 
} custom_node_t;

struct linked_list {
    custom_node_t *head;
};

struct list_node *new_node(size_t value) {
    custom_node_t *node = (custom_node_t *)malloc(sizeof(custom_node_t));
    if (node) {
        node->value = value;
        node->next = NULL;
        node->metadata = 0;
    }
    return (struct list_node *)node;
}

void insert_at_head(struct linked_list *list, size_t value) {
    if (!list) return;
    custom_node_t *node = (custom_node_t *)new_node(value);
    if (node) {
        node->next = list->head;
        list->head = node;
    }
}

void insert_at_tail(struct linked_list *list, size_t value) {
    if (!list) return;
    custom_node_t *node = (custom_node_t *)new_node(value);
    if (node) {
        if (!list->head) {
            list->head = node;
        } else {
            custom_node_t *current = list->head;
            while (current->next) {
                current = current->next;
            }
            current->next = node;
        }
    }
}

size_t remove_from_head(struct linked_list *list) {
    if (!list || !list->head) return 0;
    custom_node_t *temp = list->head;
    size_t value = temp->value;
    list->head = temp->next;
    free(temp);
    return value;
}

size_t remove_from_tail(struct linked_list *list) {
    if (!list || !list->head) return 0;
    if (!list->head->next) {
        size_t value = list->head->value;
        free(list->head);
        list->head = NULL;
        return value;
    }
    custom_node_t *current = list->head;
    custom_node_t *previous = NULL;
    while (current->next) {
        previous = current;
        current = current->next;
    }
    size_t value = current->value;
    previous->next = NULL;
    free(current);
    return value;
}

void free_list(struct linked_list list) {
    custom_node_t *current = list.head;
    while (current) {
        custom_node_t *next = current->next;
        free(current);
        current = next;
    }
    // Note: We are passing the list by value, so we cannot directly set list.head to NULL here.
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
    fprintf(fp, "[ ");
    for (struct list_node *cur = (struct list_node *)list.head; cur != NULL; cur = cur->next) {
        fprintf(fp, "%zu ", cur->value);
    }
    fprintf(fp, "]\n");
}
