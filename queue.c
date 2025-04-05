#include "queue.h"
#include "tile_game.h"
#include <stdlib.h>

// adds game state to back of queue
void enqueue(struct queue *q, struct game_state state) {
    size_t serialized = serialize(state);
    insert_at_tail(&q->list, serialized);
}

// removes and returns game state from front of queue
struct game_state dequeue(struct queue *q) {
    size_t serialized = remove_from_head(&q->list);
    return deserialize(serialized);
}

// finds shortest path to solve the tiles game
int number_of_moves(struct game_state start) {
    // already solved
    if (is_solved(start)) {
        return 0;
    }

    struct queue q = {0};
    enqueue(&q, start);

    // bfs to explore all possible moves
    while (q.list.head != NULL) {
        struct game_state current = dequeue(&q);

        // try sliding in all 4 directions
        for (int direction = 0; direction < 4; direction++) {
            if (can_slide(current, direction)) {
                struct game_state next = slide(current, direction);
                if (is_solved(next)) {
                    free_list(q.list);
                    return next.number_of_moves;
                }
                enqueue(&q, next);
            }
        }
    }

    free_list(q.list);
    return -1; // shouldnt happen if puzzle is solvable
}
