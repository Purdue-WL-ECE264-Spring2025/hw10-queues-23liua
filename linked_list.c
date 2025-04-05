#include "linked_list.h"
#include <stdlib.h>

// makes a new node with given value
struct list_node *new_node(size_t value) {
    struct list_node *node = malloc(sizeof(struct list_node));
    if (node == NULL) {
        return NULL;
    }
    node->value = value;
    node->next = NULL;
    return node;
}

// adds node to front of list
void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (node == NULL) {
        return;
    }
    node->next = list->head;
    list->head = node;
    if (list->tail == NULL) {
        list->tail = node;
    }
}

// adds node to end of list
void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (node == NULL) {
        return;
    }
    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
}

// removes and returns first node's value
size_t remove_from_head(struct linked_list *list) {
    if (list->head == NULL) {
        return 0;
    }
    struct list_node *node = list->head;
    size_t value = node->value;
    list->head = node->next;
    if (list->head == NULL) {
        list->tail = NULL;
    }
    free(node);
    return value;
}

// removes and returns last node's value
size_t remove_from_tail(struct linked_list *list) {
    if (list->tail == NULL) {
        return 0;
    }
    if (list->head == list->tail) {
        size_t value = list->head->value;
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        return value;
    }
    struct list_node *prev = NULL;
    struct list_node *curr = list->head;
    while (curr->next != NULL) {
        prev = curr;
        curr = curr->next;
    }
    size_t value = curr->value;
    free(curr);
    prev->next = NULL;
    list->tail = prev;
    return value;
}

// frees all nodes in the list
void free_list(struct linked_list list) {
    struct list_node *curr = list.head;
    while (curr != NULL) {
        struct list_node *next = curr->next;
        free(curr);
        curr = next;
    }
}
