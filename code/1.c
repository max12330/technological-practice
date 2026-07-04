#include<stdio.h>
#include<stdlib.h>
#include<time.h>
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

    //вывод probabilities
    for (int k = 0; k < m; k++) {
        printf("P(%d) = %f\n", k, probabilities[k]);
    }

    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        float rnd = (float)rand()/RAND_MAX;

        if (rnd < probabilities[0]) {
            sample[i] = 0;
        }
        else if (rnd < probabilities[0] + probabilities[1]) {
            sample[i] = 1;
        }
        else if (rnd < probabilities[0] + probabilities[1] + probabilities[2]) {
            sample[i] = 2;
        }
        else if (rnd < probabilities[0] + probabilities[1] + probabilities[2] + 
            probabilities[3]) {
            sample[i] = 3;
        }
        else if (rnd < probabilities[0] + probabilities[1] + probabilities[2] + 
            probabilities[3] + probabilities[4]) {
            sample[i] = 4;
        }
        else if (rnd < probabilities[0] + probabilities[1] + probabilities[2] + 
            probabilities[3] + probabilities[4] + probabilities[5]) {
            sample[i] = 5;
        }
        else {
            sample[i] = 6;
        }
    }

    /*
    //вывод выборки
    for (int i = 0; i < N; i++) {
        printf("%d ", sample[i]);
    }
    printf("\n");
    */

    //вывод количества каждого значения в выборке
    int *counts = (int *)calloc(m, sizeof(int));
    for (int i = 0; i < N; i++) {
        counts[sample[i]]++;
    }

    //вывод количества каждого значения в выборке
    for (int k = 0; k < m; k++) {
        printf("Count(%d) = %d\n", k, counts[k]);
    }

    //освобождение памяти
    free(sample);
    free(probabilities);
    free(counts);

    return 0;
}