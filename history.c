#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

/* Makes a full copy of the map and stores it in memory (source is map, changed for easier readibility) */
/* Returns the copy, or Null if memory allocation failed */
static int **copy_map(int sizeR, int sizeC, int **source)
{
    int **destination;
    int r;

    destination = (int **)malloc(sizeR * sizeof(int *)); /* Allocate memory for the row pointers */
    if (destination == NULL)
    {
        return NULL;
    }

    for (r = 0; r < sizeR; r++)
    {
        destination[r] = (int *)malloc(sizeC * sizeof(int)); /* Allocate memory for each row */
        if (destination[r] == NULL)
        {
            /* Free any rows already allocated before it */
            while (r > 0)
            {
                r = r - 1;
                free(destination[r]);
            }
            free(destination);
            return NULL;
        }
        memcpy(destination[r], source[r], sizeC * sizeof(int)); /* Copy the row data from source to destination */
    }

    return destination;
}

/* Frees the map that was made with copy_map */
static void free_map_copy(int sizeR, int **map)
{
    int r;

    if (map == NULL)
    {
        return;
    }
    for (r = 0; r < sizeR; r++)
    {
        free(map[r]); /* Free each row */
    }
    free(map); /* Free the array of row pointers */
}

/* Sets up the history for it to be ready to use */
void history_init(struct History *history)
{
    history -> top = NULL; /* No states saved */
    history -> size = 0; /* No nodes in the stack */
}

/* Saves the current game state as a new entry at the top of the history */
int history_push(struct History *history,
                int sizeR, int sizeC, int **map, int playerR, int playerC,
                int goalR, int goalC, int treasureR, int treasureC,
                int enemyR, int enemyC, int enemyAggro, int enemyDirection,
                int playerHasTreasure, int tileBelow)
{
    struct HistoryNode *node; /* The new node to be added to the stack */
    int **mapCopy; /* A copy of the map to be stored in the node */

    /* Allocate memory for a new node */
    node = (struct HistoryNode *)malloc(sizeof(struct HistoryNode));
    if (node == NULL)
    {
        fprintf(stderr, "couldn't allocate memory for the history node\n");
        return 0;
    }

    /* Copy the map so this snapshot is independent of the live map */
    mapCopy = copy_map(sizeR, sizeC, map);
    if (mapCopy == NULL)
    {
        fprintf(stderr, "couldn't copy map\n");
        free(node);
        return 0;
    }

    /* Save everything in the game state to the node */
    node -> state.sizeR = sizeR;
    node -> state.sizeC = sizeC;
    node -> state.map = mapCopy;
    node -> state.playerR = playerR;
    node -> state.playerC = playerC;
    node -> state.goalR = goalR;
    node -> state.goalC = goalC;
    node -> state.treasureR = treasureR;
    node -> state.treasureC = treasureC;
    node -> state.enemyR = enemyR;
    node -> state.enemyC = enemyC;
    node -> state.enemyAggro = enemyAggro;
    node -> state.enemyDirection = enemyDirection;
    node -> state.playerHasTreasure = playerHasTreasure;
    node -> state.tileBelow = tileBelow;

    /* Put the new node at the top of the stack */
    node -> prev = history -> top; /* The previous top node is now the second node in the stack */
    history -> top = node; /* Update the top pointer to the new node */
    history -> size++; /* Increase the size of the stack */

    return 1;
}

/* Restores the most recent saved state and removes it from the history */
/*   Returns 1 if it worked, 0 if there was nothing to undo */
int history_undo(struct History *history,
                int *sizeR, int *sizeC, int ***map, int *playerR, int *playerC,
                int *goalR, int *goalC, int *treasureR, int *treasureC,
                int *enemyR, int *enemyC, int *enemyAggro, int *enemyDirection,
                int *playerHasTreasure, int *tileBelow)
{
    struct HistoryNode *node; /* The node to be undone */
    struct GameState *s; /* The game state stored in that node */

    if (history -> top == NULL)
    {
        printf("Nothing to undo.\n");
        return 0;
    }

    node = history -> top;
    s = &node -> state;

    /* Free the current live map before replacing it with the saved one */
    free_map_copy(*sizeR, *map);

    /* Copy all saved values back into the game variables */
    *sizeR = s -> sizeR;
    *sizeC = s -> sizeC;
    *map = s -> map;   /* The caller now owns this map */
    *playerR = s -> playerR;
    *playerC = s -> playerC;
    *goalR = s -> goalR;
    *goalC = s -> goalC;
    *treasureR = s -> treasureR;
    *treasureC = s -> treasureC;
    *enemyR = s -> enemyR;
    *enemyC = s -> enemyC;
    *enemyAggro = s -> enemyAggro;
    *enemyDirection = s -> enemyDirection;
    *playerHasTreasure = s -> playerHasTreasure;
    *tileBelow = s -> tileBelow;

    /* Remove the node from the stack and free it */
    /* The map is owned by the caler so free later */
    history -> top = node -> prev;
    history -> size--;
    free(node);

    return 1;
}

/* Frees all history nodes and the saved data */
void history_free(struct History *history)
{
    struct HistoryNode *current; /* The node currently being freed */
    struct HistoryNode *prev; /* The previous node */

    current = history -> top; /* Start at the top of the stack */
    while (current != NULL)
    {
        prev = current -> prev; /* Save the previous node before freeing the current one */
        free_map_copy(current -> state.sizeR, current -> state.map);
        free(current);
        current = prev;
    }

    history -> top  = NULL;
    history -> size = 0;
}