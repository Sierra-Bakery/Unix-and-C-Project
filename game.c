#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "display.h"
#include "random.h"

static void move_enemy(int **map,
                       int *enemyR, int *enemyC)
{
    int newR;
    int newC;
    int blocked;
    int input;
    int inputValid = 0; /* 0 for invalid input, 1 for valid input */
 
    newR = *enemyR;
    newC = *enemyC;
 
    while (inputValid == 0)
    {
        input = random_int(0, 3);
        if (input == 0)
        {
            newR = *enemyR - 1;
        }
        else if (input == 1) 
        { 
            newR = *enemyR + 1; 
        }
        else if (input == 2) 
        { 
            newC = *enemyC - 1; 
        }
        else if (input == 3) 
        { 
            newC = *enemyC + 1; 
        }

        blocked = (map[newR][newC] == 1);
 
        if (!blocked)
        {
            map[*enemyR][*enemyC] = 0;
            map[newR][newC]         = 4;
            *enemyR = newR;
            *enemyC = newC;
            inputValid = 1;
        }
    }
}

static void move_player(int **map,
                        int *playerR, int *playerC,
                        char input)
{
    int newR;
    int newC;
    int blocked;
    int inputValid = 0; /* 0 for invalid input, 1 for valid input */
 
    newR = *playerR;
    newC = *playerC;

    while (inputValid == 0)
    {
        scanf(" %c", &input);
        if (input == 'w')
        {
            newR = *playerR - 1;
        }
        else if (input == 's') 
        { 
            newR = *playerR + 1; 
        }
        else if (input == 'a') 
        { 
            newC = *playerC - 1; 
        }
        else if (input == 'd') 
        { 
            newC = *playerC + 1; 
        }
        
        blocked = (map[newR][newC] == 1);
 
        if (!blocked)
        {
            map[*playerR][*playerC] = 0;
            map[newR][newC]         = 4;
            *playerR = newR;
            *playerC = newC;
            inputValid = 1;
        }
    }

 

}

/* Processes one tick of the game */
int gameTick(int **map, int playerR, int playerC, int goalR,
             int goalC, int treasureR, int treasureC, int enemyR,
             int enemyC, int enemyAggro, int playerHasTreasure, int *continueGame)
{
    char input;
    int inputValid = 0; /* 0 for invalid input, 1 for valid input */




}