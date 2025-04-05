#include "queue.h"
#include "linked_list.h"
#include "tile_game.h"

#include <stdbool.h>
#include <stdlib.h>

// helper that checks if a serialized state is in a linked list
static bool in_list(struct linked_list *lst, size_t val) {
    for (struct list_node *cur = lst->head; cur != NULL; cur = cur->next) {
        if (cur->value == val) {
            return true;
        }
    }
    return false;
}

// enqueue: serialize and insert at tail
void enqueue(struct queue *q, struct game_state state) {
    size_t encoded = serialize(state);
    insert_at_tail(&q->list, encoded);
}

// dequeue: remove from head and deserialize
struct game_state dequeue(struct queue *q) {
    size_t encoded = remove_from_head(&q->list);
    return deserialize(encoded);
}

// returns how many moves are needed to reach the solved state, or -1 if not found
int number_of_moves(struct game_state start) {
    // define the solved state
    struct game_state solved;
    int tile = 1;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            solved.board[r][c] = tile++;
        }
    }
    // last spot is 0
    solved.board[3][3] = 0;
    solved.empty_r = 3;
    solved.empty_c = 3;
    solved.moves = 0;

    size_t solved_int = serialize(solved);
    size_t start_int  = serialize(start);

    // check if we are already solved
    if (start_int == solved_int) {
        return 0;
    }

    // set up queue
    struct queue q;
    q.list.head = NULL;
    enqueue(&q, start);

    // track visited states
    struct linked_list visited;
    visited.head = NULL;
    insert_at_tail(&visited, start_int);

    // bfs
    while (q.list.head != NULL) {
        struct game_state current = dequeue(&q);
        size_t current_int = serialize(current);

        // if solved, free and return
        if (current_int == solved_int) {
            free_list(q.list);      // free nodes in queue
            free_list(visited);     // free nodes in visited
            return current.moves;
        }

        int r = current.empty_r;
        int c = current.empty_c;

        // 4 possible slides: from above, below, left, right

        // slide tile in from above
        if (r > 0) {
            struct game_state next = current;
            next.board[r][c] = next.board[r - 1][c];
            next.board[r - 1][c] = 0;
            next.empty_r = r - 1;
            next.empty_c = c;
            next.moves = current.moves + 1;

            size_t next_int = serialize(next);
            if (!in_list(&visited, next_int)) {
                insert_at_tail(&visited, next_int);
                enqueue(&q, next);
            }
        }

        // slide tile in from below
        if (r < 3) {
            struct game_state next = current;
            next.board[r][c] = next.board[r + 1][c];
            next.board[r + 1][c] = 0;
            next.empty_r = r + 1;
            next.empty_c = c;
            next.moves = current.moves + 1;

            size_t next_int = serialize(next);
            if (!in_list(&visited, next_int)) {
                insert_at_tail(&visited, next_int);
                enqueue(&q, next);
            }
        }

        // slide tile in from the left
        if (c > 0) {
            struct game_state next = current;
            next.board[r][c] = next.board[r][c - 1];
            next.board[r][c - 1] = 0;
            next.empty_r = r;
            next.empty_c = c - 1;
            next.moves = current.moves + 1;

            size_t next_int = serialize(next);
            if (!in_list(&visited, next_int)) {
                insert_at_tail(&visited, next_int);
                enqueue(&q, next);
            }
        }

        // slide tile in from the right
        if (c < 3) {
            struct game_state next = current;
            next.board[r][c] = next.board[r][c + 1];
            next.board[r][c + 1] = 0;
            next.empty_r = r;
            next.empty_c = c + 1;
            next.moves = current.moves + 1;

            size_t next_int = serialize(next);
            if (!in_list(&visited, next_int)) {
                insert_at_tail(&visited, next_int);
                enqueue(&q, next);
            }
        }
    }

    // if we exhaust the queue without finding a solution
    free_list(visited);
    return -1;
}
