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

    //массив теоретических вероятностей 
    //для каждого значения k от 0 до m-1
    double *probabilities = (double *)malloc(m * sizeof(double));

    for (int k = 0; k < m; k++) {
        probabilities[k] = P(m, k);
    }

    //вывод probabilities
    for (int k = 0; k < m; k++) {
        printf("P(%d) = %f\n", k, probabilities[k]);
    }

    //массив частичных сумм вероятностей
    double *cumulative_probabilities = (double *)malloc(m * sizeof(double));
    cumulative_probabilities[0] = probabilities[0];
    for (int k = 1; k < m; k++) {
        cumulative_probabilities[k] = cumulative_probabilities[k-1] + probabilities[k];
    }

    srand(time(NULL));

    //генерация выборки
    for (int i = 0; i < N; i++) {
        double rnd = (double)rand() / RAND_MAX;

        int k = 0;
        while (rnd >= cumulative_probabilities[k]) {
            k++;
        }
        sample[i] = k;
    }

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
    free(cumulative_probabilities);
    free(counts);

    return 0;
}