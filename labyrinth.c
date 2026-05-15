#include <stdio.h>
#include "filehandler.h"
#include "display.h"
#include "entities.h"
#include "game.h"
#include "random.h"
#include "history.h"
#include "graphics.h"
#include <termios.h>

/* Welcome to the game! */
/* This is the main file for the game. It processes the map file, finds entity positions, and displays the map. */
/* It also contains the main game loop */
/* Made by Dylan Baker over May 2026 For my assignment on COMP1000 - Unix and C Programming */

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

static void get_input(char *input)
{
    disableBuffer();
    scanf(" %c", input);
    enableBuffer();
}

int main(int argc, char *argv[]) /* Takes command line arguments to processes the map file */
{
    /* Diagnostics */
    int printDiagnostics = 0; /* Set it to 1 enable diagnostics, 0 to disable. */
    int result = 0;
    int continueGame = 1; /* Set it to 0 to end the game, 1 to continue. */
    /* Map dimensions */
    int sizeR;
    int sizeC;
    int **map;
    /* Entity positions */
    int playerR, playerC;
    int goalR, goalC;
    int treasureR, treasureC;
    int enemyR, enemyC;
    /* Entity attributes */
    int enemyAggro = 0; /* 0 for passive, 1 for aggressive */
    int enemyDirection = 0; /* 0 for up, 1 for down, 2 for left, 3 for right */
    int playerHasTreasure = 0; /* 0 for no treasure, 1 for has treasure */
    int tileBelow = 0; /* Value of the tile the enemy is standing on */
    int blockedMovement = 0; /* 0 for no blocked movement, 1 for blocked movement */
    /* User input */
    char input;
    /* History structure */
    struct History history;
    /* Map load result */
    int ok;

    if (argc != 2) /* Check if the correct number of command line arguments is provided */
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]);
        result = 1;
    }
    else
    {
        initRandom();

        ok = handle_file(argv[1], &sizeR, &sizeC, &map);
        find_entities(sizeR, sizeC, map, &playerR, &playerC, &goalR, &goalC, &treasureR, &treasureC, &enemyR, &enemyC);

        play_intro();

        history_init(&history);
        history_push(&history, sizeR, sizeC, map, playerR, playerC, goalR, goalC,
                        treasureR, treasureC, enemyR, enemyC,
                        enemyAggro, enemyDirection, playerHasTreasure, tileBelow);

        if (printDiagnostics)
        {
            printf("Player: %d, %d\n", playerR, playerC);
            printf("Goal: %d, %d\n", goalR, goalC);
            printf("Treasure: %d, %d\n", treasureR, treasureC);
            printf("Enemy: %d, %d\n", enemyR, enemyC);
            printf("Enemy Aggro: %d\n", enemyAggro);
            printf("Player Has Treasure: %d\n", playerHasTreasure);
        }

        while (ok && continueGame)
        {
            display_map(sizeR, sizeC, map, enemyAggro, enemyDirection);
            blockedMovement = 0;
            get_input(&input);

            if (input == 'u')
            {
                history_undo(&history, &sizeR, &sizeC, &map,
                                &playerR, &playerC,
                                &goalR, &goalC,
                                &treasureR, &treasureC,
                                &enemyR, &enemyC,
                                &enemyAggro, &enemyDirection,
                                &playerHasTreasure, &tileBelow);
            }
            else if (input == 'w' || input == 'a' || input == 's' || input == 'd')
            {
                gameTick(map, sizeR, sizeC, &playerR, &playerC,
                            goalR, goalC, treasureR, treasureC,
                            &enemyR, &enemyC, &enemyAggro, &playerHasTreasure,
                            &continueGame, &enemyDirection, &tileBelow, &input, &blockedMovement);
                if (!blockedMovement)
                {
                    history_push(&history, sizeR, sizeC, map, playerR, playerC, goalR, goalC,
                                    treasureR, treasureC, enemyR, enemyC,
                                    enemyAggro, enemyDirection, playerHasTreasure, tileBelow);
                }
            }
        }

        printf("Game Over!\n");
        free_map(sizeR, map);
        history_free(&history);
    }

    return result;
}

/* when i awake the morning, i will be a better programmer than i am now. */