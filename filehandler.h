#ifndef FILEHANDLER_H
#define FILEHANDLER_H

/* shows 1 for working and 0 for failure */
int handle_file(const char *filename, int *sizeR, int *sizeC, int ***map);

/* free memory from the map data */
void free_map(int sizeR, int **map);

#endif /* FILEHANDLER_H */