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
        return (2+sqrt(2))*(1-cos(x));
    else
        return 1.0;
}

//обратная функция распределения F^-1
double F_inv(double y) {
    if (y < 0 || y > 1) {
        printf("Error: y must be in [0, 1]\n");
        exit(1);
    }
    return acos(1 - y / (2 + sqrt(2)));
}

int main() {
    //русский язык в консоли в кодировке UTF-8
    setlocale(LC_ALL, "Russian_Russia.65001");

    //математические ожидание и дисперсия теоретические
    double theoretical_mean = (sqrt(2)+1)*(1-M_PI/4);
    double theoretical_variance = 2*(sqrt(2)+1)*(1+M_PI/4-M_PI*M_PI/32)-
        4-2*sqrt(2)-(3+2*sqrt(2))*(1-M_PI/4)*(1-M_PI/4);


    printf("Enter N\n");
    int N;
    scanf("%d", &N);


    //массив для хранения сгенерированных случайных чисел
    double* sample = (double*)malloc(N * sizeof(double));
    
    //генерация выборки
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        double u = (double)rand() / RAND_MAX; 
        sample[i] = F_inv(u);
    }

/*ПРОВЕРИТЬ ВЕСЬ КОД НИЖЕ*/


    //нахождение выборочного математического ожидания и дисперсии
    double sample_mean = 0;
    for (int i = 0; i < N; i++) {
        sample_mean += sample[i];
    }
    sample_mean /= N;

    double sample_variance = 0;
    for (int i = 0; i < N; i++) {
        sample_variance += (sample[i] - sample_mean) * (sample[i] - sample_mean);
    }
    sample_variance /= N;

    printf("Sample mean: %lf\n", sample_mean);
    printf("Sample variance: %lf\n", sample_variance);

    //вывод теоретических значений
    printf("Theoretical mean: %lf\n", theoretical_mean);
    printf("Theoretical variance: %lf\n", theoretical_variance);

    //проверка хи-квадрат критерия согласия
    int k = (int)(1 + 3.322 * log10(N)); //формула Стерджеса

    //границы интервалов
    double* boundaries = (double*)malloc((k + 1) * sizeof(double));
    boundaries[0] = 0;
    for (int i = 1; i < k; i++) {
        boundaries[i] = F_inv((double)i / k);
    }
    boundaries[k] = M_PI/4;

    //подсчет частот попадания в интервалы
    int* frequencies = (int*)calloc(k, sizeof(int));
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < k; j++) {
            if (sample[i] >= boundaries[j] && sample[i] < boundaries[j + 1]) {
                frequencies[j]++;
                break;
            }
        }
    }

    //вывод частот
    printf("Frequencies:\n");
    for (int i = 0; i < k; i++) {
        printf("Interval [%lf, %lf): %d\n", boundaries[i], boundaries[i + 1], frequencies[i]);
    }

    //вывод теоретических частот
    printf("Theoretical frequencies:\n");
    for (int i = 0; i < k; i++) {
        double theoretical_frequency = N * (F(boundaries[i + 1]) - F(boundaries[i]));
        printf("Interval [%lf, %lf): %lf\n", boundaries[i], boundaries[i + 1], theoretical_frequency);
    }
    //вычисление статистики хи-квадрат
    double chi_square_statistic = 0;
    for (int i = 0; i < k; i++) {
        double diff = frequencies[i] - N * (F(boundaries[i + 1]) - F(boundaries[i]));
        chi_square_statistic += diff * diff / (N * (F(boundaries[i + 1]) - F(boundaries[i])));
    }

    printf("Chi-square statistic: %lf\n", chi_square_statistic);

    //освобождение памяти
    free(sample);
    free(boundaries);
    free(frequencies);

    return 0;
}