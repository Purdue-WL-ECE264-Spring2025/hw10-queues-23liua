#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

struct list_node *new_node(size_t val) {
    struct list_node *p = malloc(sizeof(struct list_node));
    if (p) {
        p->value = val;
        p->next  = NULL;
    }
    return p;
}

void insert_at_head(struct linked_list *lst, size_t num) {
    struct list_node *spot = new_node(num);
    if (!spot) return;
    spot->next = lst->head;
    lst->head  = spot;
}

void insert_at_tail(struct linked_list *lst, size_t num) {
    struct list_node *fresh = new_node(num);
    if (!fresh) return;
    if (!lst->head) {
        lst->head = fresh;
        return;
    }
    struct list_node *scan = lst->head;
    while (scan->next) {
        scan = scan->next;
    }
    scan->next = fresh;
}

size_t remove_from_head(struct linked_list *lst) {
    if (!lst->head) return 0;
    struct list_node *victim = lst->head;
    size_t payload = victim->value;
    lst->head      = victim->next;
    free(victim);
    return payload;
}

size_t remove_from_tail(struct linked_list *lst) {
    if (!lst->head) return 0;
    if (!lst->head->next) {
        size_t onlyVal = lst->head->value;
        free(lst->head);
        lst->head = NULL;
        return onlyVal;
    }
    struct list_node *slow = lst->head;
    struct list_node *prev = NULL;
    while (slow->next) {
        prev = slow;
        slow = slow->next;
    }
    size_t ret = slow->value;
    free(slow);
    if (prev) {
        prev->next = NULL;
    }
    return ret;
}

void free_list(struct linked_list lst) {
    struct list_node *runner = lst.head;
    while (runner) {
        struct list_node *garbage = runner;
        runner = runner->next;
        free(garbage);
    }
}

void dump_list(FILE *fp, struct linked_list lst) {
    fprintf(fp, "( ");
    for (struct list_node *trav = lst.head; trav; trav = trav->next) {
        fprintf(fp, "%zu ", trav->value);
    }
    fprintf(fp, ")\n");
}
