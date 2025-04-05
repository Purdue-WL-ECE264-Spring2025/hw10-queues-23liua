#include "queue.h"
#include "tile_game.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// we keep the same signatures, but rename variables and mix it up

void enqueue(struct queue *q, struct game_state s) {
    uint64_t encoded = serialize(s);
    // insert at tail of q->data (linked_list)
    insert_at_tail(&(q->data), (size_t)encoded);
}

struct game_state dequeue(struct queue *q) {
    // remove head from q->data
    size_t encoded = remove_from_head(&(q->data));
    return deserialize((uint64_t)encoded);
}

// let's rename the hash size
#define BUCKET_COUNT 100003

// rename the visited_node struct
typedef struct vnode {
    uint64_t val;
    struct vnode *chain;
} vnode;

// create an array of pointers to vnode
static vnode *buckets[BUCKET_COUNT];

// do a simple mod-based hash
static int64_t hash64(uint64_t x) {
    return (int64_t)(x % BUCKET_COUNT);
}

// checks if a key is already in our buckets
static bool was_visited(uint64_t key) {
    int64_t h = hash64(key);
    vnode *trav = buckets[h];
    while (trav != NULL) {
        if (trav->val == key) {
            return true;
        }
        trav = trav->chain;
    }
    return false;
}

// inserts a new key into our visited set
static void mark_visited(uint64_t key) {
    int64_t h = hash64(key);
    vnode *nn = malloc(sizeof(vnode));
    nn->val = key;
    nn->chain = buckets[h];
    buckets[h] = nn;
}

// frees the entire visited structure
static void release_buckets() {
    for (int i = 0; i < BUCKET_COUNT; i++) {
        vnode *cur = buckets[i];
        while (cur) {
            vnode *tmp = cur->chain;
            free(cur);
            cur = tmp;
        }
        buckets[i] = NULL;
    }
}

// checks if the given state is solved
static bool puzzle_solved(struct game_state g) {
    int expect = 1;
    for (int rr = 0; rr < 4; rr++) {
        for (int cc = 0; cc < 4; cc++) {
            if (rr == 3 && cc == 3) {
                if (g.tiles[rr][cc] != 0) {
                    return false;
                }
            } else {
                if (g.tiles[rr][cc] != expect) {
                    return false;
                }
                expect++;
            }
        }
    }
    return true;
}

// frees the queue's linked_list data
static void clear_queue(struct queue *q) {
    free_list(q->data);
    q->data.head = NULL;
}

// returns the fewest moves to solve the puzzle, or -1 if impossible
int number_of_moves(struct game_state beginning) {
    // if it's already solved, return that step count
    if (puzzle_solved(beginning)) {
        return beginning.num_steps;
    }

    // init our queue
    struct queue q2;
    q2.data.head = NULL; // empty

    // clear out old visited data
    for (int k = 0; k < BUCKET_COUNT; k++) {
        buckets[k] = NULL;
    }

    // mark the start as visited
    uint64_t start_encoded = serialize(beginning);
    mark_visited(start_encoded);
    enqueue(&q2, beginning);

    // BFS
    while (q2.data.head != NULL) {
        struct game_state currentState = dequeue(&q2);
        if (puzzle_solved(currentState)) {
            release_buckets();
            clear_queue(&q2);
            return currentState.num_steps;
        }

        uint64_t cval = serialize(currentState);

        // we'll mix up the order: move_left, move_right, move_up, move_down
        // each time, we build a 'candidate' state, move it, encode it, check if visited.

        struct game_state candidate;
        uint64_t cand_encoded;

        // left
        candidate = currentState;
        move_left(&candidate);
        cand_encoded = serialize(candidate);
        if (cand_encoded != cval && !was_visited(cand_encoded)) {
            mark_visited(cand_encoded);
            enqueue(&q2, candidate);
        }

        // right
        candidate = currentState;
        move_right(&candidate);
        cand_encoded = serialize(candidate);
        if (cand_encoded != cval && !was_visited(cand_encoded)) {
            mark_visited(cand_encoded);
            enqueue(&q2, candidate);
        }

        // up
        candidate = currentState;
        move_up(&candidate);
        cand_encoded = serialize(candidate);
        if (cand_encoded != cval && !was_visited(cand_encoded)) {
            mark_visited(cand_encoded);
            enqueue(&q2, candidate);
        }

        // down
        candidate = currentState;
        move_down(&candidate);
        cand_encoded = serialize(candidate);
        if (cand_encoded != cval && !was_visited(cand_encoded)) {
            mark_visited(cand_encoded);
            enqueue(&q2, candidate);
        }
    }

    // if we exhaust possibilities without finding a solution
    release_buckets();
    return -1;
}
