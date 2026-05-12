#include <stdio.h>
#include "filehandler.h"
#include "display.h"
#include "entities.h"

/* Welcome to the game! */
/* This is the main file for the game. It processes the map file, finds entity positions, and displays the map. */
/* It also contains the main game loop */
/* Made by Dylan Baker over May 2026 For my assignment on COMP1000 - Unix and C Programming */

int main(int argc, char *argv[]) /* Takes command line arguments to processes the map file */
{
    /* Diagnostics */
    int printDiagnostics = 1; /* Set it to 1 enable diagnostics, 0 to disable. */
    int result;
    /* Map dimensions */
    int sizeR;
    int sizeC;
    int **map;
    /* Entity positions */
    int playerR,   playerC;
    int goalR,     goalC;
    int treasureR, treasureC;
    int enemyR,    enemyC;

    if (argc != 2) /* Check if the correct number of command line arguments is provided */
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]); /* Print usage message if command line arguments are not correct */
        result = 1;
    }
    else
    {
        int ok = handle_file(argv[1], &sizeR, &sizeC, &map); /* Process the map and store the data. Returns 1 if it worked*/
 
        if (ok)
        {
            find_entities(sizeR, sizeC, map, &playerR, &playerC, &goalR, &goalC, &treasureR, &treasureC, &enemyR, &enemyC);
            if (printDiagnostics)
            {
                printf("Player: %d, %d\n", playerR, playerC);
                printf("Goal: %d, %d\n", goalR, goalC);
                printf("Treasure: %d, %d\n", treasureR, treasureC);
                printf("Enemy: %d, %d\n", enemyR, enemyC);
            }
            display_map(sizeR, sizeC, map);
            free_map(sizeR, map);
            result = 0;
        }
        else
        {
            result = 1;
        }
    }

    return result;
}

/* when i awake the morning, i will be a better programmer than i am now. */