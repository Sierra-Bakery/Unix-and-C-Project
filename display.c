#include <stdio.h>
#include "display.h"
#include "colour.h"

/*
    Map tile legend:
        0  = empty cell  (plain background)
        1  = wall        (white background, no character)
        2  = goal        (green background, grey foreground 'g')
        3  = treasure    (yellow background, grey foreground 'T')
        4  = player      (blue foreground 'P')
        5  = enemy       (red foreground '>')
*/
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
        printf("g");
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
void display_map(MapData *data)
{
    int r;

    print_border(data->sizeC);

    for (r = 0; r < data->sizeR; r++) /* Renders each row of the map */
    {
        int c;
        colour_reset();
        printf("*");

        for (c = 0; c < data->sizeC; c++) /* Renders each cell in the row */
        {
            render_cell(data->map[r][c]);
            colour_reset();
        }

        colour_reset();
        printf("*\n");
    }

    print_border(data->sizeC);
}