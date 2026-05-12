#ifndef ENTITIES_H
#define ENTITIES_H

/* Finds entity positions. */
void find_entities(int sizeR, int sizeC, int **map,
                   int *playerR, int *playerC,
                   int *goalR,   int *goalC,
                   int *treasureR, int *treasureC,
                   int *enemyR,  int *enemyC);

#endif /* ENTITIES_H */