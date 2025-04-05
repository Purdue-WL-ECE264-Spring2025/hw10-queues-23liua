#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>

static struct list_node *allocate_node(size_t data) {
    struct list_node *n = (struct list_node *)malloc(sizeof(struct list_node));
    if (n) {
        n->value = data;
        n->next = NULL;
    }
    return n;
}

void insert_at_head(struct linked_list *lst, size_t value) {
    if (!lst) return;
    struct list_node *n = allocate_node(value);
    if (!n) return;
    n->next = lst->head;
    lst->head = n;
}

void insert_at_tail(struct linked_list *lst, size_t value) {
    if (!lst) return;
    struct list_node *n = allocate_node(value);
    if (!n) return;

    if (!lst->head) {
        lst->head = n;
        return;
    }

    for (struct list_node *ptr = lst->head; ptr; ptr = ptr->next) {
        if (!ptr->next) {
            ptr->next = n;
            break;
        }
    }
}

size_t remove_from_head(struct linked_list *lst) {
    if (!lst || !lst->head) return 0;

    struct list_node *to_remove = lst->head;
    size_t extracted = to_remove->value;

    lst->head = to_remove->next;
    free(to_remove);
    return extracted;
}

size_t remove_from_tail(struct linked_list *lst) {
    if (!lst || !lst->head) return 0;

    struct list_node *cur = lst->head;

    if (!cur->next) {
        size_t val = cur->value;
        free(cur);
        lst->head = NULL;
        return val;
    }

    while (cur->next && cur->next->next) {
        cur = cur->next;
    }

    size_t val = cur->next->value;
    free(cur->next);
    cur->next = NULL;
    return val;
}

void free_list(struct linked_list lst) {
    struct list_node *walker = lst.head;
    while (walker) {
        struct list_node *temp = walker;
        walker = walker->next;
        free(temp);
    }
}

void dump_list(FILE *fp, struct linked_list lst) {
    if (!fp) return;
    fprintf(fp, "< ");
    for (struct list_node *p = lst.head; p != NULL; p = p->next) {
        fprintf(fp, "%zu ", p->value);
    }
    fprintf(fp, ">\n");
}
