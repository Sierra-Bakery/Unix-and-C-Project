#include <stdio.h>
#include <stdlib.h>
#include "filehandler.h"

int handle_file(const char *filename, MapData *data)
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
        fscanf(fp, "%d %d", &data->sizeR, &data->sizeC); /* Read the number of rows and columns from the file */

        data->map = (int **)malloc(data->sizeR * sizeof(int *)); /* Allocate memory for the map based on the number of rows */

        for (r = 0; r < data->sizeR; r++)
        {
            int c;
            data->map[r] = (int *)malloc(data->sizeC * sizeof(int)); /* Allocate memory for each row of the map */

            for (c = 0; c < data->sizeC; c++)
            {
                fscanf(fp, "%d", &data->map[r][c]); /* Read each element of the map from the file */
            }
        }

        fclose(fp);
        success = 1; /* Set success to 1 if the file was read successfully */
    }

    return success;
}

void free_map(MapData *data)
{
    int r;

    for (r = 0; r < data->sizeR; r++)
    {
        free(data->map[r]); /* Free memory allocated for each row of the map */\
        data->map[r] = NULL; /* Set the pointer to NULL after freeing */
    }

    free(data->map);
    data->map = NULL;
}