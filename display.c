#include <stdio.h>
#include "display.h"
#include "colour.h"

/* Prints the border of the map */
static void print_border(int sizeC)
{
    int i;
    colour_reset();
    printf("*");
    for (i = 0; i < sizeC; i++)
    {
        printf("*");
    }
    printf("*\n");
}
/* Renders a single cell based on its type */
static void render_cell(int tile, int enemyAggro)
{
    if (tile == 1) /* Wall */
    {
        colour_set_background("white");
        printf(" ");
        colour_reset();
    }
    else if (tile == 2) /* Goal */
    {
        colour_set_background("green");
        colour_set_foreground("white");
        printf("G");
        colour_reset();
    }
    else if (tile == 3) /* Treasure */
    {
        colour_set_background("yellow");
        colour_set_foreground("white");
        printf("T");
        colour_reset();
    }
    else if (tile == 4) /* Player */
    {
        colour_set_foreground("blue");
        printf("P");
        colour_reset();
    }
    else if (tile == 5) /* Enemy */
    {
        if (enemyAggro == 1) /* If the enemy is aggressive, make it white with a red background */
        {
            colour_set_background("red");
            colour_set_foreground("white");
        }
        else
        {
            colour_set_foreground("red");
        }
        printf(">");
        colour_reset();
    }
    else /* Empty cell */
    {
        printf(" ");
    }
}

/* Displays the game map */
void display_map(int sizeR, int sizeC, int **map, int enemyAggro)
{
    int r;

    print_border(sizeC);

    for (r = 0; r < sizeR; r++) /* Renders each row of the map */
    {
        int c;
        colour_reset();
        printf("*");

        for (c = 0; c < sizeC; c++) /* Renders each cell in the row */
        {
            render_cell(map[r][c], enemyAggro);
            colour_reset();
        }
        printf("*\n");
        colour_reset();
    }

    print_border(sizeC);

    printf("Press 'w' to move UP\n");
    printf("Press 's' to move DOWN\n");
    printf("Press 'a' to move LEFT\n");
    printf("Press 'd' to move RIGHT\n");
    printf("Press 'u' to move UNDO\n");
}