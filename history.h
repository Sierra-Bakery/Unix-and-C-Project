#ifndef HISTORY_H
#define HISTORY_H
 
/* Snapshot of all game state for one turn */
typedef struct GameState
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
} GameState;

/* Linked list node: Node = turn */
typedef struct HistoryNode
{
    GameState           state;
    struct HistoryNode *prev; /* Points toward the start of the game */
} HistoryNode;
 
/* The history stack — holds a pointer to the most recent node */
typedef struct
{
    HistoryNode *top;
    int          size;
} History;
 
/* Initialise an empty history. Must be called before any other function. */
void history_init(History *h);
 
/* Push a snapshot of the current game state onto the history stack.
   Call this once per turn, AFTER all moves for that turn are complete.
   Returns 1 on success, 0 on allocation failure. */
int history_push(History *h,
                 int sizeR, int sizeC, int **map,
                 int playerR, int playerC,
                 int goalR,   int goalC,
                 int treasureR, int treasureC,
                 int enemyR,    int enemyC,
                 int enemyAggro, int enemyDirection,
                 int playerHasTreasure);
 
/* Pop the most recent state off the stack and restore it into the
   provided game variables. The map array is freed and replaced with
   the saved deep copy.
   Returns 1 if a state was restored, 0 if already at the start. */
int history_undo(History *h,
                 int *sizeR, int *sizeC, int ***map,
                 int *playerR, int *playerC,
                 int *goalR,   int *goalC,
                 int *treasureR, int *treasureC,
                 int *enemyR,    int *enemyC,
                 int *enemyAggro, int *enemyDirection,
                 int *playerHasTreasure);
 
/* Free all nodes and their map copies. Call on game exit. */
void history_free(History *h);


#endif /* HISTORY_H */