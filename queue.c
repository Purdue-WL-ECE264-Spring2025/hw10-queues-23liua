#include "queue.h"
#include "linked_list.h"
#include "tile_game.h"

#include <stdbool.h>
#include <stdlib.h>

// adds a state to the end of the queue
void enqueue(struct queue *q, struct game_state state) {
    size_t encoded = serialize(state);
    insert_at_tail(&q->list, encoded);
}

// removes and returns the state at the front of the queue
struct game_state dequeue(struct queue *q) {
    size_t encoded = remove_from_head(&q->list);
    return deserialize(encoded);
}

// checks if a serialized state is already in the list
static bool in_list(struct linked_list *lst, size_t val) {
    for (struct list_node *cur = lst->head; cur != NULL; cur = cur->next) {
        if (cur->value == val) {
            return true;
        }
    }
    return false;
}

// returns the fewest moves needed to solve the puzzle, or -1 if it can't find a solution
int number_of_moves(struct game_state start) {
    // set up the solved state
    struct game_state solved;
    int tile = 1;
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            solved.board[r][c] = tile++;
        }
    }
    solved.board[3][3] = 0;
    solved.empty_r = 3;
    solved.empty_c = 3;
    solved.moves = 0;

    size_t solved_int = serialize(solved);
    size_t start_int = serialize(start);

    // if the starting state is already solved, return 0
    if (start_int == solved_int) {
        return 0;
    }

    // set up the queue
    struct queue q;
    q.list.head = NULL;
    q.list.tail = NULL;
    enqueue(&q, start);

    // keep track of visited states
    struct linked_list visited;
    visited.head = NULL;
    visited.tail = NULL;
    insert_at_tail(&visited, start_int);

    // bfs loop
    while (q.list.head != NULL) {
        struct game_state current = dequeue(&q);
        size_t current_int = serialize(current);

        // check if we reached the solved state
        if (current_int == solved_int) {
            free_list(q.list);
            free_list(visited);
            return current.moves;
        }

        int r = current.empty_r;
        int c = current.empty_c;

        // move tile from above
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

        // move tile from below
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

        // move tile from the left
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

        // move tile from the right
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

    // if we somehow never reach a solution
    free_list(visited);
    return -1;
}
