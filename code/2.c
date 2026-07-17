#define _USE_MATH_DEFINES
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<locale.h>
//программа для моделирования непрерывной случайной величины с функцией распределения F

//функция распределения F
double F(double x) {
    if (x < 0)
        return 0.0;
    else if (x < M_PI/4)
        return (2+sqrt(2))*(1-cos(x)); // F(x) = (2+sqrt(2))*(1-cos(x)) для 0 <= x < pi/4
    else
        return 1.0; // F(x) = 1 для x >= pi/4
}


int main() {
    //русский язык в консоли в кодировке UTF-8
    setlocale(LC_ALL, "Russian_Russia.65001");

    printf("Enter N\n");
    int N;
    scanf("%d", &N);

    printf("Enter m\n");
    int m;
    scanf("%d", &m);
    if (m < 2) {
        printf("m должно быть >= 2\n");
        return 1;
    }

    //выборка из N случайных величин
    //int *sample = (int *)malloc(N * sizeof(int));

    //массив теоретических вероятностей 
    //для каждого значения k от 0 до m-1
    double *probabilities = (double *)malloc(m * sizeof(double));
    
    //массив частичных сумм вероятностей
    double *cumulative_probabilities = (double *)malloc(m * sizeof(double));

    //матожидание теоретическое
    double theoretical_mean = 0;
    //матожидание теоретическое квадрата случайной величины
    double theoretical_mean_pow2 = 0;

    //дисперсия теоретическая
    double theoretical_variance = 0;

    //вычисление теоретических характеристик
    for (int k = 0; k < m; k++) {
        probabilities[k] = P(m, k);
        theoretical_mean += k * probabilities[k];
        theoretical_mean_pow2 += k * k * probabilities[k];
        if (k == 0)
            cumulative_probabilities[0] = probabilities[0];
        else
            cumulative_probabilities[k] = cumulative_probabilities[k-1] +
                                            probabilities[k];
    }
    theoretical_variance = theoretical_mean_pow2 - 
            theoretical_mean * theoretical_mean;

    /*
    //вывод probabilities
    for (int k = 0; k < m; k++) {
        printf("P(%d) = %f\n", k, probabilities[k]);
    }
    */

    //выборочные матожидание и дисперсия
    double sum_x2 = 0.0;         // сумма квадратов значений выборки
    double sample_mean = 0;      // выборочное матожидание
    double sample_variance = 0;  // выборочная дисперсия
    //количество каждого значения в выборке
    int *counts = (int *)calloc(m, sizeof(int));

    //генерация выборки
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        double rnd = (double)rand() / RAND_MAX;

        int k = 0;
        while (rnd > cumulative_probabilities[k] && k < m - 1) {
            k++;
        }
        //sample[i] = k;
        
        counts[k]++;
        sample_mean += k;
        sum_x2 += (double)k * k;
    }

    sample_mean /= N;
    sample_variance = (sum_x2 / N) - (sample_mean * sample_mean);


    //вывод теоретических и выборочных характеристик
    printf("Theoretical mean = %f\n", theoretical_mean);
    printf("Sample mean = %f\n", sample_mean);
    printf("Theoretical variance = %f\n", theoretical_variance);
    printf("Sample variance = %f\n", sample_variance);

    //проверка гипотезы о соответствии распределения выборки
    //теоретическому распределению
    //распределение хи-квадрат
    double chi_squared = 0;
    for (int k = 0; k < m; k++) {
        double expected_count = N * probabilities[k];
        chi_squared += pow(counts[k] - expected_count, 2) / expected_count;
    }
    printf("Chi-squared = %f\n", chi_squared);

    // Критические значения для alpha = 0.05 и df от 1 до 30
    double critical_values[] = {
        3.84, 5.99, 7.81, 9.49, 11.07, 12.59, 14.07, 15.51, 16.92, 18.31,
        19.68, 21.03, 22.36, 23.68, 24.99, 26.30, 27.59, 28.87, 30.14, 31.41,
        32.67, 33.92, 35.17, 36.42, 37.65, 38.89, 40.11, 41.34, 42.56, 43.77
    };

    int df = m - 1; // степени свободы
    if (df < 1 || df > 30) {
        printf("Критическое значение для степени свободы=%d не предусмотрено.\n", df);
        return 1;
    }

    double chi_crit = critical_values[df - 1];
    if (chi_squared < chi_crit)
        printf("Гипотеза принимается (x^2 = %.3f < %.3f)\n", chi_squared, chi_crit);
    else
        printf("Гипотеза отвергается (x^2 = %.3f >= %.3f)\n", chi_squared, chi_crit);

    //освобождение памяти
    //free(sample);
    free(probabilities);
    free(cumulative_probabilities);
    free(counts);

    return 0;
}