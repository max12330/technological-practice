#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>


//вероятность того, что случайная величина примет значение k при параметре m
double P(int m, int k) {
    return (sqrt(k+1) - sqrt(k))/sqrt(m);
}


int main() {
    int m = 5;
    int N = 100;
    double *probabilities = (double *)malloc(m * sizeof(double));
    for (int k = 0; k < m; k++) {
        probabilities[k] = P(m, k);
    }
    int *sample = (int *)malloc(N * sizeof(int));


    srand(time(NULL));
    

    for (int i = 0; i < N; i++) {
        float rnd = (float)rand()/RAND_MAX;
        int k = 1;

        while (rnd > 0) {
            rnd -= probabilities[k];
            k+=1;
        }
        sample[i] = k;
    }

    //вывод выборки
    for (int i = 0; i < N; i++) {
        printf("%d ", sample[i]);
    }
    printf("\n");

    //количество каждого значения в выборке
    int *counts = (int *)calloc(m, sizeof(int));
    for (int i = 0; i < N; i++) {
        counts[sample[i]]++;
    }

    //вывод количества каждого значения в выборке
    for (int k = 0; k < m; k++) {
        printf("Count(%d) = %d\n", k, counts[k]);
    }

    //матожидание теоретичекое
    double theoretical_mean = 0;
    for (int k = 0; k < m; k++) {
        theoretical_mean += k * probabilities[k];
    }
    printf("Theoretical mean = %f\n", theoretical_mean);

    //матожидание выборки
    double sample_mean = 0;
    for (int i = 0; i < N; i++) {
        sample_mean += sample[i];
    }
    sample_mean /= N;
    printf("Sample mean = %f\n", sample_mean);

    //дисперсия теоретическая
    double theoretical_variance = 0;
    for (int k = 0; k < m; k++) {
        theoretical_variance += probabilities[k] * pow(k - theoretical_mean, 2);
    }
    printf("Theoretical variance = %f\n", theoretical_variance);

    //дисперсия выборки
    double sample_variance = 0;
    for (int i = 0; i < N; i++) {
        sample_variance += pow(sample[i] - sample_mean, 2);
    }
    sample_variance /= N;
    printf("Sample variance = %f\n", sample_variance);


    //освобождение памяти
    free(sample);
    free(probabilities);

    return 0;
}