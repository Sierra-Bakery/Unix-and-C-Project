#include <stdio.h>
#include "filehandler.h"
#include "display.h"
#include "entities.h"
#include "game.h"
#include "random.h"
#include "history.h"
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
    int result;
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
    int tileBelow = 0;    /* Value of the tile the enemy is standing on */
    /* User input */
    char input;

    void initRandom(void); /* Initialize the random number generator */

    if (argc != 2) /* Check if the correct number of command line arguments is provided */
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]); /* Print usage message if command line arguments are not correct */
        result = 1;
    }
    else /* Starts the game */
    {
        int ok = handle_file(argv[1], &sizeR, &sizeC, &map); /* Process the map and store the data. Returns 1 if it worked*/
        find_entities(sizeR, sizeC, map, &playerR, &playerC, &goalR, &goalC, &treasureR, &treasureC, &enemyR, &enemyC);
        struct History history;
        history_init(&history);
        /* Save the initial state to the history */
        history_push(&history, sizeR, sizeC, map, playerR, playerC, goalR, goalC, treasureR, treasureC, enemyR, enemyC, enemyAggro, enemyDirection, playerHasTreasure, tileBelow);

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
            else if (input == 'w' || input == 'a' || input == 's' || input == 'd') /* If the user wants to move: process the game tick and save the new state to the history */
            {
                gameTick(map, sizeR, sizeC, &playerR, &playerC,
                        goalR, goalC, treasureR, treasureC,
                        &enemyR, &enemyC, &enemyAggro, &playerHasTreasure,
                        &continueGame, &enemyDirection, &tileBelow, &input);
                history_push(&history, sizeR, sizeC, map, playerR, playerC, goalR, goalC, treasureR, treasureC, enemyR, enemyC, enemyAggro, enemyDirection, playerHasTreasure, tileBelow);
            }
        }

    printf("Game Over!\n");
    free_map(sizeR, map);
    history_free(&history); /* Free the history */
    }

    return result;
}

/* when i awake the morning, i will be a better programmer than i am now. */