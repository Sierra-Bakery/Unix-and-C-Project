#ifndef GAME_H
#define GAME_H

int gameTick(int **map, int rows, int cols, int *playerR, int *playerC,
             int goalR, int goalC, int treasureR, int treasureC,
             int *enemyR, int *enemyC, int enemyAggro, int *playerHasTreasure,
             int *continueGame);

#endif /* GAME_H */