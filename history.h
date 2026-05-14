#ifndef HISTORY_H
#define HISTORY_H

/* Snapshot of all game data for one turn using a structure */
struct GameState
{
    /* Map dimensions */
    int sizeR;
    int sizeC;
    int **map; /* the copy of the full map */
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
    int tileBelow;
};

/* Linked list node, each node = one turn */
struct HistoryNode
{
    struct GameState    state;
    struct HistoryNode *prev; /* Points toward the start of the game (in the stack) */
};

/* History stack, points to the most recent node in the stack */
struct History
{
    struct HistoryNode *top;
    int                 size;
};

/* Initialise an empty history */
void history_init(struct History *history);

/* Push a snapshot of the current game to the history stack */
/* Returns 1 if worked amd 0 if it couldnt allocate memory */
int history_push(struct History *history, int sizeR, int sizeC, int **map,
                int playerR, int playerC, int goalR,   int goalC,
                int treasureR, int treasureC, int enemyR,    int enemyC,
                int enemyAggro, int enemyDirection, int playerHasTreasure, int tileBelow);

/* Pop the most recent state and restore it. The map array is freed and replaced with the next copy */
/* Returns 1 if restored and 0 if already at the start. */
int history_undo(struct History *history,
                int *sizeR, int *sizeC, int ***map, int *playerR, int *playerC,
                int *goalR,   int *goalC, int *treasureR, int *treasureC,
                int *enemyR,    int *enemyC, int *enemyAggro, int *enemyDirection,
                int *playerHasTreasure, int *tileBelow);

/* Free all nodes and the map copies */
void history_free(struct History *history);

#endif /* HISTORY_H */