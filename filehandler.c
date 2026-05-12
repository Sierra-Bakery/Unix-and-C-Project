#include <stdio.h>
#include <stdlib.h>
#include "filehandler.h"

int handle_file(const char *filename, int *sizeR, int *sizeC, int ***map)
{
    FILE *fp;
    int success;
    int r;

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Error: Could not open file \"%s\"\n", filename); /* Print error message if file cannot be opened */
        success = 0;
    }
    else
    {
        fscanf(fp, "%d %d", sizeR, sizeC); /* Read the number of rows and columns from the file */

        *map = (int **)malloc(*sizeR * sizeof(int *)); /* Allocate memory for the map based on the number of rows */

        for (r = 0; r < *sizeR; r++)
        {
            int c;
            (*map)[r] = (int *)malloc(*sizeC * sizeof(int)); /* Allocate memory for each row of the map */

            for (c = 0; c < *sizeC; c++)
            {
                fscanf(fp, "%d", &(*map)[r][c]); /* Read each element of the map from the file */
            }
        }

        fclose(fp);
        success = 1; /* Set success to 1 if the file was read successfully */
    }

    return success;
}

void free_map(int sizeR, int **map)
{
    int r;

    for (r = 0; r < sizeR; r++)
    {
        free(map[r]); /* Free memory allocated for each row of the map */\
        map[r] = NULL; /* Set the pointer to NULL after freeing */
    }

    free(map);
    map = NULL;
}