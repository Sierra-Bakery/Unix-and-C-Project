#ifndef HISTORY_H
#define HISTORY_H

/* Snapshot of all game state for one turn */
struct GameState
{
    /* Map dimensions */
    int sizeR;
    int sizeC;
    int **map; /* Deep copy of the full map */

    /* Entity positions */
    int playerR;
    int playerC;
    int goalR;
    int goalC;
    int treasureR;
    int treasureC;
    int enemyR;
    int enemyC;

    /* Entity attributes */
    int enemyAggro;
    int enemyDirection;
    int playerHasTreasure;
    int tileBelow; /* What is under the enemy (0=empty, 2=goal, 3=treasure) */
};

/* Linked list node: each node is one turn of history */
struct HistoryNode
{
    struct GameState    state;
    struct HistoryNode *prev; /* Points toward the start of the game */
};

/* The history stack: holds a pointer to the most recent node */
struct History
{
    struct HistoryNode *top;
    int                 size;
};

/* Initialise an empty history. Must be called before any other function. */
void history_init(struct History *h);

/* Push a snapshot of the current game state onto the history stack.
   Call this once per turn, AFTER all moves for that turn are complete.
   Returns 1 on success, 0 on allocation failure. */
int history_push(struct History *h,
                 int sizeR, int sizeC, int **map,
                 int playerR, int playerC,
                 int goalR,   int goalC,
                 int treasureR, int treasureC,
                 int enemyR,    int enemyC,
                 int enemyAggro, int enemyDirection,
                 int playerHasTreasure, int tileBelow);

/* Pop the most recent state off the stack and restore it into the
   provided game variables. The map array is freed and replaced with
   the saved deep copy.
   Returns 1 if a state was restored, 0 if already at the start. */
int history_undo(struct History *h,
                 int *sizeR, int *sizeC, int ***map,
                 int *playerR, int *playerC,
                 int *goalR,   int *goalC,
                 int *treasureR, int *treasureC,
                 int *enemyR,    int *enemyC,
                 int *enemyAggro, int *enemyDirection,
                 int *playerHasTreasure, int *tileBelow);

/* Free all nodes and their map copies. Call on game exit. */
void history_free(struct History *h);

#endif /* HISTORY_H */