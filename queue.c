#include "queue.h"
#include "tile_game.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

void enqueue(struct queue *qq, struct game_state st) {
    uint64_t coded = serialize(st);
    insert_at_tail(&(qq->data), (size_t)coded);
}

struct game_state dequeue(struct queue *qq) {
    size_t sVal = remove_from_head(&(qq->data));
    return deserialize((uint64_t)sVal);
}

#define VISITSIZE 88801

typedef struct visit_node {
    uint64_t rep;
    struct visit_node *chain;
} visit_node;

static visit_node *visitTable[VISITSIZE];

static unsigned get_index(uint64_t key) {
    return (unsigned)(key % VISITSIZE);
}

static bool visited_has(uint64_t key) {
    unsigned i = get_index(key);
    for (visit_node *cursor = visitTable[i]; cursor; cursor = cursor->chain) {
        if (cursor->rep == key) return true;
    }
    return false;
}

static void visited_add(uint64_t key) {
    unsigned i = get_index(key);
    visit_node *tmp = malloc(sizeof(visit_node));
    if (!tmp) exit(2);
    tmp->rep   = key;
    tmp->chain = visitTable[i];
    visitTable[i] = tmp;
}

static void visited_cleanup() {
    for (int j = 0; j < VISITSIZE; j++) {
        visit_node *ptr = visitTable[j];
        while (ptr) {
            visit_node *gone = ptr;
            ptr = ptr->chain;
            free(gone);
        }
        visitTable[j] = NULL;
    }
}

static bool is_solved(struct game_state s) {
    int need = 1;
    for (int rr = 0; rr < 4; rr++) {
        for (int cc = 0; cc < 4; cc++) {
            if (rr == 3 && cc == 3) {
                if (s.tiles[rr][cc] != 0) return false;
            } else {
                if (s.tiles[rr][cc] != need) return false;
                need++;
            }
        }
    }
    return true;
}

static void flush_queue(struct queue *qq) {
    free_list(qq->data);
    qq->data.head = NULL;
}

int number_of_moves(struct game_state start) {
    if (is_solved(start)) {
        return start.num_steps;
    }
    for (int c = 0; c < VISITSIZE; c++) {
        visitTable[c] = NULL;
    }
    struct queue store;
    store.data.head = NULL;
    uint64_t origin = serialize(start);
    visited_add(origin);
    enqueue(&store, start);
    while (store.data.head) {
        struct game_state top = dequeue(&store);
        if (is_solved(top)) {
            visited_cleanup();
            flush_queue(&store);
            return top.num_steps;
        }
        uint64_t top_id = serialize(top);
        struct game_state attempt;
        uint64_t next_id;

        move_down(& (attempt = top)); 
        next_id = serialize(attempt);
        if (next_id != top_id && !visited_has(next_id)) {
            visited_add(next_id);
            enqueue(&store, attempt);
        }
        move_right(& (attempt = top));
        next_id = serialize(attempt);
        if (next_id != top_id && !visited_has(next_id)) {
            visited_add(next_id);
            enqueue(&store, attempt);
        }
        move_up(& (attempt = top));
        next_id = serialize(attempt);
        if (next_id != top_id && !visited_has(next_id)) {
            visited_add(next_id);
            enqueue(&store, attempt);
        }
        move_left(& (attempt = top));
        next_id = serialize(attempt);
        if (next_id != top_id && !visited_has(next_id)) {
            visited_add(next_id);
            enqueue(&store, attempt);
        }
    }
    visited_cleanup();
    return -1;
}
