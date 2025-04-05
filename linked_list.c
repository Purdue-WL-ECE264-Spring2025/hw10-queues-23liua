#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

struct list_node *new_node(size_t val) {
    // allocate memory for a new node
    struct list_node *temp = malloc(sizeof(struct list_node));
    if (temp) {
        temp->value = val;
        temp->next  = NULL;
    }
    return temp;
}

void insert_at_head(struct linked_list *lst, size_t data) {
    struct list_node *temp = new_node(data);
    if (!temp) return; 
    temp->next = lst->head;
    lst->head  = temp;
}

void insert_at_tail(struct linked_list *lst, size_t data) {
    struct list_node *fresh = new_node(data);
    if (fresh == NULL) return;

    if (lst->head == NULL) {
        // empty list
        lst->head = fresh;
    } else {
        // walk to the end
        struct list_node *p = lst->head;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = fresh;
    }
}

size_t remove_from_head(struct linked_list *lst) {
    if (lst->head == NULL) return 0; // nothing to remove
    struct list_node *temp = lst->head;
    size_t val = temp->value;
    lst->head = temp->next;
    free(temp);
    return val;
}

size_t remove_from_tail(struct linked_list *lst) {
    if (lst->head == NULL) return 0;  // nothing here
    if (lst->head->next == NULL) {
        // only one node
        size_t val = lst->head->value;
        free(lst->head);
        lst->head = NULL;
        return val;
    }

    // otherwise, find the node before the last one
    struct list_node *walk = lst->head;
    struct list_node *prev = NULL;
    while (walk->next != NULL) {
        prev = walk;
        walk = walk->next;
    }
    size_t val = walk->value;
    free(walk);
    if (prev) {
        prev->next = NULL;
    }
    return val;
}

void free_list(struct linked_list lst) {
    // free all nodes
    struct list_node *ptr = lst.head;
    while (ptr != NULL) {
        struct list_node *advance = ptr->next;
        free(ptr);
        ptr = advance;
    }
}

void dump_list(FILE *fp, struct linked_list lst) {
    fprintf(fp, "[ ");
    for (struct list_node *scan = lst.head; scan != NULL; scan = scan->next) {
        fprintf(fp, "%zu ", scan->value);
    }
    fprintf(fp, "]\n");
}
