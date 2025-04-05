#include "queue.h"
#include "tile_game.h"
#include "linked_list.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define VISITSIZE 88801
#define MAX_NEIGHBORS 4

typedef struct visit_node {
    uint64_t state_hash;
    struct visit_node *next;
} visit_node;

static visit_node *visited[VISITSIZE];

static unsigned hash_state(uint64_t key) {
    return (unsigned)(key % VISITSIZE);
}

static bool state_visited(uint64_t key) {
    visit_node *node = visited[hash_state(key)];
    while (node) {
        if (node->state_hash == key) return true;
        node = node->next;
    }
    return false;
}

static void mark_visited(uint64_t key) {
    unsigned index = hash_state(key);
    visit_node *new_node = malloc(sizeof(*new_node));
    if (!new_node) exit(2);
    new_node->state_hash = key;
    new_node->next = visited[index];
    visited[index] = new_node;
}

static void clear_visited() {
    for (int i = 0; i < VISITSIZE; i++) {
        visit_node *node = visited[i];
        while (node) {
            visit_node *temp = node;
            node = node->next;
            free(temp);
        }
        visited[i] = NULL;
    }
}

static bool is_solved(struct game_state s) {
    int need = 1;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            if (r == 3 && c == 3) {
                if (s.tiles[r][c] != 0) return false;
            } else if (s.tiles[r][c] != need++) {
                return false;
            }
        }
    }
    return true;
}

static int generate_successors(struct game_state state, struct game_state neighbors[]) {
    struct game_state temp;
    int count = 0;
    
    temp = state;
    if (move_left(&temp)) {
        neighbors[count++] = temp;
    }
    
    temp = state;
    if (move_right(&temp)) {
        neighbors[count++] = temp;
    }
    
    temp = state;
    if (move_up(&temp)) {
        neighbors[count++] = temp;
    }
    
    temp = state;
    if (move_down(&temp)) {
        neighbors[count++] = temp;
    }
    
    return count;
}

void enqueue(struct queue *q, struct game_state state) {
    uint64_t val = serialize(state);
    insert_at_tail(&q->data, val);
}

struct game_state dequeue(struct queue *q) {
    uint64_t val = remove_from_head(&q->data);
    return deserialize(val);
}

int number_of_moves(struct game_state start) {
    if (is_solved(start)) return start.num_steps;
    
    // Initialize visited set
    for (int i = 0; i < VISITSIZE; i++) visited[i] = NULL;
    
    struct queue q = {0};
    mark_visited(serialize(start));
    enqueue(&q, start);

    while (q.data.head) {
        struct game_state current = dequeue(&q);
        
        if (is_solved(current)) {
            int moves = current.num_steps;
            clear_visited();
            free_list(q.data);
            return moves;
        }

        struct game_state neighbors[MAX_NEIGHBORS];
        int count = generate_successors(current, neighbors);

        for (int i = 0; i < count; i++) {
            uint64_t neighbor_hash = serialize(neighbors[i]);
            if (!state_visited(neighbor_hash)) {
                mark_visited(neighbor_hash);
                enqueue(&q, neighbors[i]);
            }
        }
    }

    clear_visited();
    return -1;
}
