#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

// makes a new node with the given value
struct list_node *new_node(size_t value) {
    struct list_node *node = malloc(sizeof(struct list_node));
    node->value = value;
    node->next = NULL;
    return node;
}

// inserts a node with the given value at the head of the list
void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    node->next = list->head;
    list->head = node;
}

// inserts a node with the given value at the tail of the list
void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);

    // if list is empty, the new node becomes head
    if (list->head == NULL) {
        list->head = node;
        return;
    }

    // otherwise, find the last node
    struct list_node *cur = list->head;
    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = node;
}

// removes a node from the head of the list and returns its value
size_t remove_from_head(struct linked_list *list) {
    if (list->head == NULL) {
        return 0; // or some sentinel for "empty list"
    }
    struct list_node *temp = list->head;
    size_t val = temp->value;
    list->head = temp->next;  // advance head
    free(temp);
    return val;
}

// removes a node from the tail of the list and returns its value
size_t remove_from_tail(struct linked_list *list) {
    if (list->head == NULL) {
        return 0; // empty list
    }

    // if there's only one node in the list
    if (list->head->next == NULL) {
        size_t val = list->head->value;
        free(list->head);
        list->head = NULL;
        return val;
    }

    // otherwise, find the node before the last node
    struct list_node *cur = list->head;
    while (cur->next->next != NULL) {
        cur = cur->next;
    }
    size_t val = cur->next->value;
    free(cur->next);
    cur->next = NULL;
    return val;
}

// frees every node in the list
void free_list(struct linked_list list) {
    struct list_node *cur = list.head;
    while (cur != NULL) {
        struct list_node *next = cur->next;
        free(cur);
        cur = next;
    }
}

// prints
void dump_list(FILE *fp, struct linked_list list) {
    fprintf(fp, "[ ");
    for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
        fprintf(fp, "%zu ", cur->value);
    }
    fprintf(fp, "]\n");
}
