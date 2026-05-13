#include<stdlib.h>
#include<time.h>
#include"random.h"

/* NOTE: This file contains functions from Curtin University's Assignment supplementary materials */
/* Credit - Curtin University Assignment supplementary materials: Random Number Generator.zip */
void initRandom()
{
    srand(time(NULL));
}

int random_int(int min, int max)
{
    int number = -1;

    if(min <= max)
    {
        number = (rand() % (max-min+1)) + min;
    }

    return number;
}