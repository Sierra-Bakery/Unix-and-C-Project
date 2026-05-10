#include <stdio.h>
#include "filehandler.h"

int main(int argc, char *argv[])
{
    int result;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]);
        result = 1;
    }
    else
    {
        result = handle_file(argv[1]);

        if (result == 1)
        {
            result = 0; /* success → exit code 0 */
        }
        else
        {
            result = 1; /* failure → exit code 1 */
        }
    }

    return result;
}
