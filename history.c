#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "history.h"

/* Copies map and stores it to memory */
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
            /* Free already-allocated rows before returning */
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

/* Free a deep-copied map */
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

void history_init(History *h)
{
    h->top  = NULL;
    h->size = 0;
}

int history_push(History *h,
                 int sizeR, int sizeC, int **map,
                 int playerR, int playerC,
                 int goalR,   int goalC,
                 int treasureR, int treasureC,
                 int enemyR,    int enemyC,
                 int enemyAggro, int enemyDirection,
                 int playerHasTreasure)
{
    HistoryNode *node;
    int **mapCopy;

    node = (HistoryNode *)malloc(sizeof(HistoryNode));
    if (node == NULL)
    {
        fprintf(stderr, "history_push: out of memory\n");
        return 0;
    }

    mapCopy = copy_map(sizeR, sizeC, map);
    if (mapCopy == NULL)
    {
        fprintf(stderr, "history_push: failed to copy map\n");
        free(node);
        return 0;
    }

    /* Fill the snapshot */
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

    /* Push onto the stack */
    node->prev = h->top;
    h->top     = node;
    h->size++;

    return 1;
}

int history_undo(History *h,
                 int *sizeR, int *sizeC, int ***map,
                 int *playerR, int *playerC,
                 int *goalR,   int *goalC,
                 int *treasureR, int *treasureC,
                 int *enemyR,    int *enemyC,
                 int *enemyAggro, int *enemyDirection,
                 int *playerHasTreasure)
{
    HistoryNode *node;
    GameState   *s;

    if (h->top == NULL)
    {
        printf("Nothing to undo.\n");
        return 0;
    }

    node = h->top;
    s    = &node->state;

    /* Free the current live map before replacing it */
    free_map_copy(*sizeR, *map);

    /* Restore all game variables from the snapshot */
    *sizeR             = s->sizeR;
    *sizeC             = s->sizeC;
    *map               = s->map;   /* Hand ownership of the copy to the caller */
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

    /* Pop the node */
    h->top = node->prev;
    h->size--;
    free(node); /* The map itself is now owned by the caller; don't free it */

    return 1;
}

void history_free(History *h)
{
    HistoryNode *current;
    HistoryNode *prev;

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