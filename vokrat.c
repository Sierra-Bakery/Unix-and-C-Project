#include <stdio.h>
#include "filehandler.h"
#include "display.h"

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
        int ok = handle_file(argv[1], &data);
 
        if (ok)
        {
            display_map(&data);
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

/* when i awake the morning, i will be a better programmer than i am now. */