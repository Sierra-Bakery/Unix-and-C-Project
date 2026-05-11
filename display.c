#include <stdio.h>
#include "display.h"
#include "color.h"

/* Renders a single cell based on its map value */
static void draw_cell(int value)
{
    switch (value)
    {
        case 1: /* Wall: white background, space */
            setBackground("white");
            printf("  ");
            resetColour();
            break;

        case 2: /* Goal: green background, grey foreground 'g' */
            setBackground("green");
            setForeground("white");
            printf(" g");
            resetColour();
            break;

        case 3: /* Treasure: yellow background, grey foreground 'T' */
            setBackground("yellow");
            setForeground("white");
            printf(" T");
            resetColour();
            break;

        case 4: /* Player: blue foreground 'P' */
            setForeground("blue");
            printf(" P");
            resetColour();
            break;

        case 5: /* Enemy: red foreground '>' */
            setForeground("red");
            printf(" >");
            resetColour();
            break;

        default: /* Empty floor: just a dark space */
            printf("  ");
            break;
    }
}

/* Draws a border row of '*' spanning the map width */
static void draw_border(int sizeC)
{
    int c;
    for (c = 0; c < (sizeC * 2) + 4; c++)
    {
        printf("*");
    }
    printf("\n");
}

void display_map(const MapData *data)
{
    int r;

    draw_border(data->sizeC);

    for (r = 0; r < data->sizeR; r++)
    {
        int c;
        printf("* ");

        for (c = 0; c < data->sizeC; c++)
        {
            draw_cell(data->map[r][c]);
        }

        printf(" *\n");
    }

    draw_border(data->sizeC);
}
