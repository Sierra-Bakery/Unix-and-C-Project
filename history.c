#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

/* Makes a full copy of the map and stores it in memory.
   Returns the copy, or NULL if memory allocation failed. */
static int **copy_map(int sizeR, int sizeC, int **src)
{
    int **dst;
    int r;

    dst = (int **)malloc(sizeR * sizeof(int *));
    if (dst == NULL)
    {
        return NULL;
    }

    for (r = 0; r < sizeR; r++)
    {
        dst[r] = (int *)malloc(sizeC * sizeof(int));
        if (dst[r] == NULL)
        {
            /* Free any rows already allocated before giving up */
            while (r > 0)
            {
                r--;
                free(dst[r]);
            }
            free(dst);
            return NULL;
        }
        memcpy(dst[r], src[r], sizeC * sizeof(int));
    }

    return dst;
}

/* Frees a map that was made with copy_map */
static void free_map_copy(int sizeR, int **map)
{
    int r;

    if (map == NULL)
    {
        return;
    }
    for (r = 0; r < sizeR; r++)
    {
        free(map[r]);
    }
    free(map);
}

/* Sets up the history so it is empty and ready to use */
void history_init(struct History *h)
{
    h->top  = NULL;
    h->size = 0;
}

/* Saves the current game state as a new entry at the top of the history */
int history_push(struct History *h,
                 int sizeR, int sizeC, int **map,
                 int playerR, int playerC,
                 int goalR,   int goalC,
                 int treasureR, int treasureC,
                 int enemyR,    int enemyC,
                 int enemyAggro, int enemyDirection,
                 int playerHasTreasure, int tileBelow)
{
    struct HistoryNode *node;
    int **mapCopy;

    /* Allocate a new node */
    node = (struct HistoryNode *)malloc(sizeof(struct HistoryNode));
    if (node == NULL)
    {
        fprintf(stderr, "history_push: out of memory\n");
        return 0;
    }

    /* Copy the map so this snapshot is independent of the live map */
    mapCopy = copy_map(sizeR, sizeC, map);
    if (mapCopy == NULL)
    {
        fprintf(stderr, "history_push: failed to copy map\n");
        free(node);
        return 0;
    }

    /* Save everything into the node */
    node->state.sizeR             = sizeR;
    node->state.sizeC             = sizeC;
    node->state.map               = mapCopy;
    node->state.playerR           = playerR;
    node->state.playerC           = playerC;
    node->state.goalR             = goalR;
    node->state.goalC             = goalC;
    node->state.treasureR         = treasureR;
    node->state.treasureC         = treasureC;
    node->state.enemyR            = enemyR;
    node->state.enemyC            = enemyC;
    node->state.enemyAggro        = enemyAggro;
    node->state.enemyDirection    = enemyDirection;
    node->state.playerHasTreasure = playerHasTreasure;
    node->state.tileBelow         = tileBelow;

    /* Place the new node at the top of the stack */
    node->prev = h->top;
    h->top     = node;
    h->size++;

    return 1;
}

/* Restores the most recent saved state and removes it from the history.
   Returns 1 if it worked, 0 if there was nothing to undo. */
int history_undo(struct History *h,
                 int *sizeR, int *sizeC, int ***map,
                 int *playerR, int *playerC,
                 int *goalR,   int *goalC,
                 int *treasureR, int *treasureC,
                 int *enemyR,    int *enemyC,
                 int *enemyAggro, int *enemyDirection,
                 int *playerHasTreasure, int *tileBelow)
{
    struct HistoryNode *node;
    struct GameState   *s;

    if (h->top == NULL)
    {
        printf("Nothing to undo.\n");
        return 0;
    }

    node = h->top;
    s    = &node->state;

    /* Free the current live map before replacing it with the saved one */
    free_map_copy(*sizeR, *map);

    /* Copy all saved values back into the game variables */
    *sizeR             = s->sizeR;
    *sizeC             = s->sizeC;
    *map               = s->map;   /* The caller now owns this map */
    *playerR           = s->playerR;
    *playerC           = s->playerC;
    *goalR             = s->goalR;
    *goalC             = s->goalC;
    *treasureR         = s->treasureR;
    *treasureC         = s->treasureC;
    *enemyR            = s->enemyR;
    *enemyC            = s->enemyC;
    *enemyAggro        = s->enemyAggro;
    *enemyDirection    = s->enemyDirection;
    *playerHasTreasure = s->playerHasTreasure;
    *tileBelow         = s->tileBelow;

    /* Remove the node from the stack and free it.
       The map belongs to the caller now so we do NOT free it here. */
    h->top = node->prev;
    h->size--;
    free(node);

    return 1;
}

/* Frees all history nodes and their saved maps */
void history_free(struct History *h)
{
    struct HistoryNode *current;
    struct HistoryNode *prev;

    current = h->top;
    while (current != NULL)
    {
        prev = current->prev;
        free_map_copy(current->state.sizeR, current->state.map);
        free(current);
        current = prev;
    }

    h->top  = NULL;
    h->size = 0;
}