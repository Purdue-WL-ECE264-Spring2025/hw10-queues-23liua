#include "linked_list.h"
#include <stdlib.h>

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
    if (list->tail == NULL) {
        list->tail = node;
    }
}

// inserts a node with the given value at the tail of the list
void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (list->tail == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
}

// removes a node from the head of the list and returns its value
size_t remove_from_head(struct linked_list *list) {
    if (list->head == NULL) {
        return 0;
    }
    struct list_node *temp = list->head;
    size_t val = temp->value;
    list->head = temp->next;
    if (list->head == NULL) {
        list->tail = NULL;
    }
    free(temp);
    return val;
}

//removes a node from the tail of the list and returns its value
size_t remove_from_tail(struct linked_list *list) {
    if (list->head == NULL) {
        return 0;
    }
    if (list->head == list->tail) {
        size_t val = list->head->value;
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        return val;
    }
    struct list_node *cur = list->head;
    while (cur->next != list->tail) {
        cur = cur->next;
    }
    size_t val = list->tail->value;
    free(list->tail);
    list->tail = cur;
    cur->next = NULL;
    return val;
}

//frees all nodes in the list
void free_list(struct linked_list list) {
    struct list_node *cur = list.head;
    while (cur != NULL) {
        struct list_node *next = cur->next;
        free(cur);
        cur = next;
    }
}

//prints
void dump_list(FILE *fp, struct linked_list list) {
    fprintf(fp, "[ ");
    for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
        fprintf(fp, "%zu ", cur->value);
    }
    fprintf(fp, "]\n");
}
