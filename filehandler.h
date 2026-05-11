#ifndef FILEHANDLER_H
#define FILEHANDLER_H

typedef struct
{
    int sizeR;
    int sizeC;
    int **map;
} MapData;

/* shows 1 for working, 0 for failure */
int handle_file(const char *filename, MapData *data);

/* free memory from the map data */
void free_map(MapData *data);

#endif /* FILEHANDLER_H */