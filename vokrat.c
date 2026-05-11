#include <stdio.h>
#include "filehandler.h"

int main(int argc, char *argv[])
{
    int result;
    MapData data;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]);
        result = 1;
    }
    else
    {
        int ok = handle_file(argv[1], &data);

        if (ok)
        {
            int r;
            printf("Rows: %d, Cols: %d\n", data.sizeR, data.sizeC);

            for (r = 0; r < data.sizeR; r++)
            {
                int c;
                for (c = 0; c < data.sizeC; c++)
                {
                    printf("%d ", data.map[r][c]);
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