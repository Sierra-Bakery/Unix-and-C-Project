#define _DEFAULT_SOURCE /* For sleep */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include "game.h"
#include "display.h"
#include "random.h"

/* NOTE: This file contains functions from Curtin University's Assignment supplementary materials */
/* Credit - Curtin University Assignment supplementary materials: How to Make a Program Accept a Char Immediately.zip */
static void disableBuffer(void)
{
    struct termios mode;

    tcgetattr(0, &mode);
    mode.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(0, TCSANOW, &mode);
}

static void enableBuffer(void)
{
    struct termios mode;

    tcgetattr(0, &mode);
    mode.c_lflag |= (ECHO | ICANON);
    tcsetattr(0, TCSANOW, &mode);
}
/* End of code from Curtin University's Assignment supplementary materials: How to Make a Program Accept a Char Immediately.zip */

static void newSleep(float timeInSeconds)
{
    struct timespec ts;
    ts.tv_sec = (int) timeInSeconds;
    ts.tv_nsec = (long)((timeInSeconds - ((int) timeInSeconds)) * 1000000000);
    nanosleep(&ts, NULL);
}

static void move_enemy(int **map, int rows, int cols,
                       int *enemyR, int *enemyC, int *enemyDirection, int *tileBelow)
{
    int newR;
    int newC;
    int blocked;
    int input;
    int inputValid = 0;

    while (inputValid == 0)
    {
        newR = *enemyR;
        newC = *enemyC;

        input = random_int(0, 3);
        if (input == 0)
        {
            newR = *enemyR - 1;
            *enemyDirection = 0;
        }
        else if (input == 1)
        {
            newR = *enemyR + 1;
            *enemyDirection = 1;
        }
        else if (input == 2)
        {
            newC = *enemyC - 1;
            *enemyDirection = 2;
        }
        else if (input == 3)
        {
            newC = *enemyC + 1;
            *enemyDirection = 3;
        }

        /* Bounds check BEFORE accessing map to prevent segfault */
        if (newR >= 0 && newR < rows && newC >= 0 && newC < cols)
        {
            blocked = (map[newR][newC] == 1);

            if (!blocked)
            {
                /* Restore the tile the enemy is leaving */
                map[*enemyR][*enemyC] = *tileBelow;

                /* Save whatever is on the destination tile before overwriting */
                *tileBelow = map[newR][newC];
                if (*tileBelow == 5)
                {
                    *tileBelow = 0; /* Shouldn't happen, but guard anyway */
                }

                map[newR][newC] = 5;
                *enemyR = newR;
                *enemyC = newC;
                inputValid = 1;
            }
        }
    }
    return;
}
 
static void move_player(int **map, int rows, int cols,
                        int *playerR, int *playerC)
{
    int newR;
    int newC;
    int blocked;
    char input;
    int inputValid = 0; /* 0 for invalid input, 1 for valid input */
 
    while (inputValid == 0)
    {
        newR = *playerR;
        newC = *playerC;
 
        disableBuffer();
        scanf(" %c", &input);
        enableBuffer();
 
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
 
        /* Bounds check BEFORE accessing map to prevent segfault */
        if (newR >= 0 && newR < rows && newC >= 0 && newC < cols)
        {
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
    return;
}

static void check_caught(int playerR, int playerC, int enemyR, int enemyC, int *continueGame)
{
    if (playerR == enemyR && playerC == enemyC)
    {
        printf("You were caught by the enemy!\n");
        *continueGame = 0;
    }
}
 
static void check_treasure(int playerR, int playerC, int treasureR, int treasureC, int *playerHasTreasure, int *enemyAggro)
{
    if (playerR == treasureR && playerC == treasureC)
    {
        printf("You found the treasure!\n");
        *playerHasTreasure = 1;
        *enemyAggro = 1;
    }
}
 
static void check_goal(int playerR, int playerC, int goalR, int goalC, int playerHasTreasure, int *continueGame)
{
    if (playerR == goalR && playerC == goalC && playerHasTreasure)
    {
        printf("You escaped with the treasure! You win!\n");
        *continueGame = 0;
    }
}

/* Processes one tick of the game */
int gameTick(int **map, int rows, int cols, int *playerR, int *playerC,
             int goalR, int goalC, int treasureR, int treasureC,
             int *enemyR, int *enemyC, int *enemyAggro, int *playerHasTreasure,
             int *continueGame, int *enemyDirection, int *tileBelow)
{
    int i;

    move_player(map, rows, cols, playerR, playerC);
    check_caught(*playerR, *playerC, *enemyR, *enemyC, continueGame);
    check_treasure(*playerR, *playerC, treasureR, treasureC, playerHasTreasure, enemyAggro);
    check_goal(*playerR, *playerC, goalR, goalC, *playerHasTreasure, continueGame);
 
    if (*enemyAggro)
    {
        for (i = 0; i < 3; i++) /* Move the enemy 3 times */
        {
            newSleep(0.1);
            move_enemy(map, rows, cols, enemyR, enemyC, enemyDirection, tileBelow);
            check_caught(*playerR, *playerC, *enemyR, *enemyC, continueGame);
            display_map(rows, cols, map, *enemyAggro, *enemyDirection);
        }
    }
    else
    {
        for (i = 0; i < 2; i++) /* Move the enemy 2 times */
        {
            newSleep(0.1);
            move_enemy(map, rows, cols, enemyR, enemyC, enemyDirection, tileBelow);
            check_caught(*playerR, *playerC, *enemyR, *enemyC, continueGame);
            display_map(rows, cols, map, *enemyAggro, *enemyDirection);
        }
    }
 
    return 1;
}