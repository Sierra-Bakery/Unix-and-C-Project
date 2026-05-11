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
        fprintf(stderr, "Error: Could not open file \"%s\"\n", filename);
        success = 0;
    }
    else
    {
        fscanf(fp, "%d %d", &data->sizeR, &data->sizeC);

        data->map = (int **)malloc(data->sizeR * sizeof(int *));

        for (r = 0; r < data->sizeR; r++)
        {
            int c;
            data->map[r] = (int *)malloc(data->sizeC * sizeof(int));

            for (c = 0; c < data->sizeC; c++)
            {
                fscanf(fp, "%d", &data->map[r][c]);
            }
        }

        fclose(fp);
        success = 1;
    }

    return success;
}

void free_map(MapData *data)
{
    int r;

    for (r = 0; r < data->sizeR; r++)
    {
        free(data->map[r]);
    }

    free(data->map);
    data->map = NULL;
}