#include "queue.h"
#include "tile_game.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define VISIT_CAPACITY 88801

typedef struct hashed_node {
    uint64_t fingerprint;
    struct hashed_node *next;
} hashed_node;

static hashed_node *seen[VISIT_CAPACITY];

static unsigned compute_hash(uint64_t key) {
    return (unsigned)(key % VISIT_CAPACITY);
}

static int has_been_seen(uint64_t fingerprint) {
    hashed_node *cur = seen[compute_hash(fingerprint)];
    while (cur) {
        if (cur->fingerprint == fingerprint) return 1;
        cur = cur->next;
    }
    return 0;
}

static void mark_seen(uint64_t fingerprint) {
    unsigned idx = compute_hash(fingerprint);
    hashed_node *entry = malloc(sizeof(hashed_node));
    if (!entry) exit(EXIT_FAILURE);
    entry->fingerprint = fingerprint;
    entry->next = seen[idx];
    seen[idx] = entry;
}

static void wipe_seen() {
    for (int i = 0; i < VISIT_CAPACITY; ++i) {
        hashed_node *cur = seen[i];
        while (cur) {
            hashed_node *to_free = cur;
            cur = cur->next;
            free(to_free);
        }
        seen[i] = NULL;
    }
}

static int state_is_goal(struct game_state candidate) {
    uint8_t goal_tiles[4][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 0}
    };
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            if (candidate.tiles[i][j] != goal_tiles[i][j])
                return 0;
    return 1;
}

void enqueue(struct queue *q, struct game_state g) {
    uint64_t code = serialize(g);
    insert_at_tail(&q->data, code);
}

struct game_state dequeue(struct queue *q) {
    uint64_t val = remove_from_head(&q->data);
    return deserialize(val);
}

int number_of_moves(struct game_state start) {
    if (state_is_goal(start))
        return start.num_steps;

    struct queue bfs;
    bfs.data.head = NULL;

    for (int i = 0; i < VISIT_CAPACITY; ++i)
        seen[i] = NULL;

    enqueue(&bfs, start);
    mark_seen(serialize(start));

    while (bfs.data.head) {
        struct game_state cur = dequeue(&bfs);
        if (state_is_goal(cur)) {
            int result = cur.num_steps;
            wipe_seen();
            free_list(bfs.data);
            return result;
        }

        struct game_state dirs[4];
        dirs[0] = cur; dirs[1] = cur;
        dirs[2] = cur; dirs[3] = cur;

        move_up(&dirs[0]);
        move_down(&dirs[1]);
        move_left(&dirs[2]);
        move_right(&dirs[3]);

        for (int i = 0; i < 4; ++i) {
            uint64_t s = serialize(dirs[i]);
            if (s != serialize(cur) && !has_been_seen(s)) {
                dirs[i].num_steps = cur.num_steps + 1;
                enqueue(&bfs, dirs[i]);
                mark_seen(s);
            }
        }
    }

    wipe_seen();
    return -1;
}
