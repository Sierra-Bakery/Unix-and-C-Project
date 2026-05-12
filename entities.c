#include "entities.h"

void find_entities(int sizeR, int sizeC, int **map,
                   int *playerR, int *playerC,
                   int *goalR,   int *goalC,
                   int *treasureR, int *treasureC,
                   int *enemyR,  int *enemyC)
{
    int r;

    *playerR   = -1; *playerC   = -1;
    *goalR     = -1; *goalC     = -1;
    *treasureR = -1; *treasureC = -1;
    *enemyR    = -1; *enemyC    = -1;

    for (r = 0; r < sizeR; r++)
    {
        int c;
        for (c = 0; c < sizeC; c++)
        {
            if (map[r][c] == 4)
            {
                *playerR = r;
                *playerC = c;
            }
            else if (map[r][c] == 2)
            {
                *goalR = r;
                *goalC = c;
            }
            else if (map[r][c] == 3)
            {
                *treasureR = r;
                *treasureC = c;
            }
            else if (map[r][c] == 5)
            {
                *enemyR = r;
                *enemyC = c;
            }
        }
    }
}