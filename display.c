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
/* Renders a single cell based on its tile type */
static void render_cell(int tile)
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
        colour_set_foreground("red");
        printf(">");
        colour_reset();
    }
    else /* Empty cell */
    {
        printf(" ");
    }
}

/* Displays the game map */
void display_map(int sizeR, int sizeC, int **map)
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
            render_cell(map[r][c]);
            colour_reset();
        }

        colour_reset();
        printf("*\n");
    }

    print_border(sizeC);
}