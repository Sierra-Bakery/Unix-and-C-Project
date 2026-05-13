#include <stdio.h>
#include "filehandler.h"
#include "display.h"
#include "entities.h"
#include "game.h"

/* Welcome to the game! */
/* This is the main file for the game. It processes the map file, finds entity positions, and displays the map. */
/* It also contains the main game loop */
/* Made by Dylan Baker over May 2026 For my assignment on COMP1000 - Unix and C Programming */

int main(int argc, char *argv[]) /* Takes command line arguments to processes the map file */
{
    /* Diagnostics */
    int printDiagnostics = 1; /* Set it to 1 enable diagnostics, 0 to disable. */
    int result;
    int continueGame = 1; /* Set it to 0 to end the game, 1 to continue. */
    /* Map dimensions */
    int sizeR;
    int sizeC;
    int **map;
    /* Entity positions */
    int playerR,   playerC;
    int goalR,     goalC;
    int treasureR, treasureC;
    int enemyR,    enemyC;
    /* Entity attributes */
    int enemyAggro = 0; /* 0 for passive, 1 for aggressive */
    int playerHasTreasure = 0; /* 0 for no treasure, 1 for has treasure */

    if (argc != 2) /* Check if the correct number of command line arguments is provided */
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]); /* Print usage message if command line arguments are not correct */
        result = 1;
    }
    else /* Starts the game */
    {
        int ok = handle_file(argv[1], &sizeR, &sizeC, &map); /* Process the map and store the data. Returns 1 if it worked*/
        find_entities(sizeR, sizeC, map, &playerR, &playerC, &goalR, &goalC, &treasureR, &treasureC, &enemyR, &enemyC);

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
            display_map(sizeR, sizeC, map, enemyAggro);
            continueGame = gameTick(map, playerR, playerC, goalR, goalC, treasureR, treasureC, enemyR, enemyC, enemyAggro, playerHasTreasure, &continueGame);
        }

    printf("Game Over!\n");
    free_map(sizeR, map);
    }

    return result;
}

/* when i awake the morning, i will be a better programmer than i am now. */