#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include<math.h>

//вероятность того, что случайная величина примет значение k при параметре m
double P(int m, int k) {
    return (sqrt(k+1) - sqrt(k))/sqrt(m);
}


int main() {
    printf("Enter N\n");
    int N;
    scanf("%d", &N);

    printf("Enter m\n");
    int m;
    scanf("%d", &m);

    //выборка из N случайных величин
    int *sample = (int *)malloc(N * sizeof(int));

    //массив вероятностей для каждого значения k от 0 до m-1
    double *probabilities = (double *)malloc(m * sizeof(double));

    for (int k = 0; k < m; k++) {
        probabilities[k] = P(m, k);
    }

    

    float rnd = (float)rand()/RAND_MAX;

    return 0;
}