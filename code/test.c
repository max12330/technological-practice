#include<stdio.h>
#include<stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));

    int rnd = rand();
    for (int i =0; i<10; i++)
    printf("rnd = %f\n", (float)rand()/RAND_MAX);

    return 0;
}