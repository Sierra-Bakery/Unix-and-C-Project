#include <stdio.h>
#include "filehandler.h"
#include "display.h"

int main(int argc, char *argv[]) /* Takes command line arguments to processes the map file */
{
    int result;
    int sizeR;
    int sizeC;
    int **map;

    if (argc != 2) /* Check if the correct number of command line arguments is provided */
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]); /* Print usage message if command line arguments are not correct */
        result = 1;
    }
    else
    {
        int ok = handle_file(argv[1], &sizeR, &sizeC, &map); /* Process the map and store the data in the map data structure. Returns 1 if it worked*/
 
        if (ok)
        {
            display_map(sizeR, sizeC, map);
            free_map(sizeR, map);
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