#include <stdio.h>
#include "filehandler.h"
#include "display.h"

int main(int argc, char *argv[]) /* Takes command line arguments to processes the map file */
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
        int ok = handle_file(argv[1], &data); /* Process the map and store the data in the map data structure. Returns 1 if it worked*/
 
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