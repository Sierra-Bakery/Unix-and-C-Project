#include <stdio.h>
#include "filehandler.h"

int handle_file(const char *filename)
{
    FILE *fp;
    int success;

    fp = fopen(filename, "r");

    if (fp == NULL)
    {
        fprintf(stderr, "Error: Could not open file \"%s\" for reading.\n", filename);
        success = 0;
    }
    else
    {
        printf("File \"%s\" opened successfully.\n", filename);
        /* Future file processing goes here */
        fclose(fp);
        success = 1;
    }

    return success;
}
