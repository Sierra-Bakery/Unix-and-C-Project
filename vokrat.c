#include <stdio.h>
#include "filehandler.h"

int main(int argc, char *argv[])
{
    int result;
    MapData data; /* Start map data structure (3 variables; sizeR, sizeC, map) */

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]); /* Print usage message if command line arguments are not correct */
        result = 1;
    }
    else
    {
        int ok = handle_file(argv[1], &data); /* Call the file handling function to read the map data */

        if (ok)
        {
            int r;
            printf("Rows: %d, Cols: %d\n", data.sizeR, data.sizeC); /* Print the number of rows and columns in the map */

            for (r = 0; r < data.sizeR; r++)
            {
                int c;
                for (c = 0; c < data.sizeC; c++)
                {
                    printf("%d ", data.map[r][c]); /* Print each element in the map */
                }
                printf("\n");
            }

            free_map(&data);
            result = 0;
        }
        else
        {
            result = 1;
        }
    }

    return result;
}