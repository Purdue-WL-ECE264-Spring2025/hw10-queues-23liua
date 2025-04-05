#include "queue.h"
#include "tile_game.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define HASH_BUCKETS 88801

typedef struct node_track {
    uint64_t fingerprint;
    struct node_track *next;
} node_track;

static node_track *visited_map[HASH_BUCKETS];

static unsigned bucket_index(uint64_t key) {
    return key % HASH_BUCKETS;
}

static bool was_visited(uint64_t hash) {
    for (node_track *cur = visited_map[bucket_index(hash)]; cur; cur = cur->next) {
        if (cur->fingerprint == hash) return true;
    }
    return false;
}

static void add_to_visited(uint64_t hash) {
    unsigned idx = bucket_index(hash);
    node_track *entry = malloc(sizeof(node_track));
    if (!entry) exit(1);
    entry->fingerprint = hash;
    entry->next = visited_map[idx];
    visited_map[idx] = entry;
}

static void reset_visited() {
    for (int i = 0; i < HASH_BUCKETS; i++) {
        node_track *cur = visited_map[i];
        while (cur) {
            node_track *tmp = cur;
            cur = cur->next;
            free(tmp);
        }
        visited_map[i] = NULL;
    }
}

static bool goal_reached(struct game_state state) {
    int val = 1;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (r == 3 && c == 3) {
                if (state.tiles[r][c] != 0) return false;
            } else {
                if (state.tiles[r][c] != val++) return false;
            }
        }
    }
    return true;
}

void enqueue(struct queue *q, struct game_state state) {
    uint64_t data = serialize(state);
    insert_at_tail(&q->data, data);
}

struct game_state dequeue(struct queue *q) {
    uint64_t encoded = remove_from_head(&q->data);
    return deserialize(encoded);
}

static void clean_queue(struct queue *q) {
    free_list(q->data);
    q->data.head = NULL;
}

int number_of_moves(struct game_state begin) {
    if (goal_reached(begin)) return begin.num_steps;

    for (int i = 0; i < HASH_BUCKETS; i++) visited_map[i] = NULL;

    struct queue q = {0};
    add_to_visited(serialize(begin));
    enqueue(&q, begin);

    while (q.data.head) {
        struct game_state current = dequeue(&q);
        if (goal_reached(current)) {
            int answer = current.num_steps;
            clean_queue(&q);
            reset_visited();
            return answer;
        }

        struct game_state copy;
        uint64_t current_hash = serialize(current);

        const int ops = 4;
        void (*moves[ops])(struct game_state *) = { move_up, move_down, move_left, move_right };

        for (int i = 0; i < ops; i++) {
            copy = current;
            moves[i](&copy);
            uint64_t next_hash = serialize(copy);
            if (next_hash != current_hash && !was_visited(next_hash)) {
                add_to_visited(next_hash);
                enqueue(&q, copy);
            }
        }
    }

    reset_visited();
    return -1;
}
