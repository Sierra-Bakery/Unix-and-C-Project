#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graphics.h"
#include "colour.h"

#define ROWS         15
#define COLS         55
#define FADE_SECONDS 5.0f  /* Total time for the red-to-orange fade */
#define FLASH_SECONDS 1.0f /* How long the final black+white flash lasts */
#define END_PAUSE    1.0f  /* Short pause after the flash before the game starts */

/* Tile states */
#define TILE_RED    0  /* Not yet faded: red background */
#define TILE_ORANGE 1  /* Faded: orange background */

#define LETTER_HEIGHT 5
#define LETTER_WIDTH  5
#define LETTER_GAP    1
#define START_ROW     5
#define START_COL     1

/* 9 letters: L A B Y R I N T H */
#define NUM_LETTERS 9
static const int LETTERS[NUM_LETTERS][LETTER_HEIGHT][LETTER_WIDTH] =
{
    { /* L */
        {1,0,0,0,0},
        {1,0,0,0,0},
        {1,0,0,0,0},
        {1,0,0,0,0},
        {1,1,1,1,1}
    },
    { /* A */
        {0,1,1,1,0},
        {1,0,0,0,1},
        {1,1,1,1,1},
        {1,0,0,0,1},
        {1,0,0,0,1}
    },
    { /* B */
        {1,1,1,1,0},
        {1,0,0,0,1},
        {1,1,1,1,0},
        {1,0,0,0,1},
        {1,1,1,1,0}
    },
    { /* Y */
        {1,0,0,0,1},
        {1,0,0,0,1},
        {0,1,1,1,0},
        {0,0,1,0,0},
        {0,0,1,0,0}
    },
    { /* R */
        {1,1,1,1,0},
        {1,0,0,0,1},
        {1,1,1,1,0},
        {1,0,1,0,0},
        {1,0,0,1,0}
    },
    { /* I */
        {1,1,1,1,1},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {1,1,1,1,1}
    },
    { /* N */
        {1,0,0,0,1},
        {1,1,0,0,1},
        {1,0,1,0,1},
        {1,0,0,1,1},
        {1,0,0,0,1}
    },
    { /* T */
        {1,1,1,1,1},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0}
    },
    { /* H */
        {1,0,0,0,1},
        {1,0,0,0,1},
        {1,1,1,1,1},
        {1,0,0,0,1},
        {1,0,0,0,1}
    }
};

/* Returns 1 if the cell (r, c) is part of a LABYRINTH letter pixel, 0 otherwise */
static int is_letter_pixel(int r, int c)
{
    int letter;
    int lr; /* Row within the letter */
    int lc; /* Column within the letter */
    int col; /* Starting column of the current letter on screen */

    if (r < START_ROW || r >= START_ROW + LETTER_HEIGHT)
    {
        return 0;
    }

    lr = r - START_ROW;

    col = START_COL;
    for (letter = 0; letter < NUM_LETTERS; letter++)
    {
        if (c >= col && c < col + LETTER_WIDTH)
        {
            lc = c - col;
            return LETTERS[letter][lr][lc];
        }
        col += LETTER_WIDTH + LETTER_GAP;
    }

    return 0;
}

/* Sleep for a given number of seconds (supports fractions) */
static void gfx_sleep(float seconds)
{
    struct timespec ts;
    ts.tv_sec  = (int)seconds;
    ts.tv_nsec = (long)((seconds - (int)seconds) * 1000000000L);
    nanosleep(&ts, NULL);
}

/* Move the terminal cursor to row r, column c (1-based) */
static void move_cursor(int r, int c)
{
    printf("\033[%d;%dH", r, c);
}

/* Hide and show the terminal cursor so it doesn't flicker during drawing */
static void hide_cursor(void)  
    {
        printf("\033[?25l");
    }
static void show_cursor(void)
    {
        printf("\033[?25h");
    }

static void draw_tile(int r, int c, int isLetter, int faded)
{
    move_cursor(r + 1, c + 1); /* +1 because terminal rows/cols are 1-based */

    if (!faded)
    {
        /* Still red: letter pixels are hidden (red on red) */
        colour_set_background("red");
        colour_set_foreground("red");
        printf(" ");
    }
    else if (isLetter)
    {
        /* Faded and is a letter: reveal it as black on orange */
        colour_set_background("black"); /* closest to orange in ANSI */
        colour_set_foreground("black");
        printf(" ");
    }
    else
    {
        /* Faded empty tile: orange */
        colour_set_background("yellow");
        colour_set_foreground("yellow");
        printf(" ");
    }

    colour_reset();
    fflush(stdout);
}

static void draw_full_screen(int mode)
{
    int r;
    int c;
    int isLetter;

    for (r = 0; r < ROWS; r++)
    {
        for (c = 0; c < COLS; c++)
        {
            isLetter = is_letter_pixel(r, c);
            move_cursor(r + 1, c + 1);

            if (mode == 0) /* All red */
            {
                colour_set_background("red");
                printf(" ");
            }
            else if (mode == 1) /* Orange fade complete */
            {
                if (isLetter)
                {
                    colour_set_background("yellow");
                    colour_set_foreground("black");
                    printf(" ");
                }
                else
                {
                    colour_set_background("yellow");
                    printf(" ");
                }
            }
            else if (mode == 2) /* Flash: black + white letters */
            {
                if (isLetter)
                {
                    colour_set_background("white");
                    colour_set_foreground("white");
                    printf(" ");
                }
                else
                {
                    colour_set_background("black");
                    printf(" ");
                }
            }

            colour_reset();
        }
    }
    fflush(stdout);
}

/* Fisher-Yates shuffle on an array of ints */
static void shuffle(int *arr, int n)
{
    int i;
    int j;
    int tmp;

    for (i = n - 1; i > 0; i--)
    {
        j = rand() % (i + 1);
        tmp    = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

void play_intro(void)
{
    int total_tiles;
    int *order;     /* Shuffled list of tile indices to fade */
    int i;
    int r;
    int c;
    long delay_ns;  /* Nanoseconds to sleep between each tile flip */
    struct timespec tile_sleep;

    total_tiles = ROWS * COLS;

    /* Build a shuffled order of all tile indices */
    order = (int *)malloc(total_tiles * sizeof(int));
    if (order == NULL)
    {
        return; /* If memory fails, skip the intro gracefully */
    }
    for (i = 0; i < total_tiles; i++)
    {
        order[i] = i;
    }
    shuffle(order, total_tiles);

    delay_ns = (long)((FADE_SECONDS * 1000000000L) / total_tiles);
    tile_sleep.tv_sec  = 0;
    tile_sleep.tv_nsec = delay_ns;

    /* Set up the terminal */
    hide_cursor();
    printf("\033[2J"); /* Clear screen */
    draw_full_screen(0); /* Start: all red */

    /* Fade each tile from red to orange in random order */
    for (i = 0; i < total_tiles; i++)
    {
        r = order[i] / COLS;
        c = order[i] % COLS;
        draw_tile(r, c, is_letter_pixel(r, c), 1);
        nanosleep(&tile_sleep, NULL);
    }

    free(order);

    /* Brief pause on the fully-faded orange screen */
    gfx_sleep(0.3f);

    /* Flash: black background, white LABYRINTH */
    draw_full_screen(2);
    gfx_sleep(FLASH_SECONDS);

    /* End pause then clear and hand off to the game */
    gfx_sleep(END_PAUSE);
    printf("\033[2J"); /* Clear screen */
    move_cursor(1, 1);
    show_cursor();
    colour_reset();
    fflush(stdout);
}