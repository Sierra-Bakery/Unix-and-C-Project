#ifndef FILEHANDLER_H
#define FILEHANDLER_H

typedef struct
{
    int sizeR;
    int sizeC;
    int **map;
} MapData;

/* Returns 1 on success, 0 on failure */
int handle_file(const char *filename, MapData *data);

/* Always call this after handle_file succeeds, to free map memory */
void free_map(MapData *data);

#endif /* FILEHANDLER_H */