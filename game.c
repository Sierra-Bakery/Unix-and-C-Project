#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "display.h"

static void move_player(int **map,
                        int *playerR, int *playerC,
                        char direction)
{
    int newR;
    int newC;
    int blocked;
 
    newR = *playerR;
    newC = *playerC;
 
    if (direction == 'w') { newR = *playerR - 1; }
    else if (direction == 's') { newR = *playerR + 1; }
    else if (direction == 'a') { newC = *playerC - 1; }
    else if (direction == 'd') { newC = *playerC + 1; }
 
    blocked = (map[newR][newC] == 1);
 
    if (!blocked)
    {
        map[*playerR][*playerC] = 0;
        map[newR][newC]         = 4;
        *playerR = newR;
        *playerC = newC;
    }
    else
    {
        printf("Blocked!\n");
    }
}

/* Processes one tick of the game */
int gameTick(int **map, int playerR, int playerC, int goalR, int goalC, int treasureR, int treasureC, int enemyR, int enemyC, int enemyAggro, int playerHasTreasure, int *continueGame)
{
    char input;
    int inputValid = 0; /* 0 for invalid input, 1 for valid input */

    while (!inputValid)
    {
        /* Get user input */
        scanf(" %c", &input);
        /* Process user input */
        switch (input)
        {
            case 'W':
            case 'w':
                /* Move player up */

            case 'S':
            case 's':
                /* Move player down */

            case 'A':
            case 'a':
                /* Move player left */

            case 'D':
            case 'd':
                /* Move player right */

            default:
                inputValid = 0; /* Invalid input */
        }
    }
    return 0;
}