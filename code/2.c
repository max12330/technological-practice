#define _USE_MATH_DEFINES
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<locale.h>


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
    int N=100000;
    //scanf("%d", &N);
    printf("N = %d\n", N);

    //проверка хи-квадрат критерия согласия
    int k = (int)(1 + 3.322 * log10(N)); //формула Стёрджеса

    //границы интервалов
    double* boundaries = (double*)malloc((k + 1) * sizeof(double));
    boundaries[0] = 0;
    for (int i = 1; i < k; i++) {
        boundaries[i] = F_inv((double)i / k);
    }
    boundaries[k] = M_PI/4;

    //подсчет частот попадания в интервалы
    int* frequencies = (int*)calloc(k, sizeof(int));

    //генерация выборки
    double sum = 0.0; //сумма для матожидания
    double sum_sq = 0.0; //сумма квадратов для дисперсии

    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        double u = (double)rand() / RAND_MAX; 
        double x = F_inv(u);
        sum += x;
        sum_sq += x * x;
        
        //подсчет частот попадания в интервалы
        int found = 0;
        for (int j = 0; j < k; j++) {
            if (x >= boundaries[j] && x < boundaries[j + 1]) {
                frequencies[j]++;
                found = 1;
                break;
            }
        }
        if (!found) {
            frequencies[k - 1]++;
        }
    }

    //нахождение выборочных математического ожидания и дисперсии
    double sample_mean = sum / N;
    double sample_variance = sum_sq / N - sample_mean * sample_mean;

    //вывод теоретических и выборочных характеристик
    printf("Sample mean: %lf\n", sample_mean);
    printf("Sample variance: %lf\n", sample_variance);
    printf("Theoretical mean: %lf\n", theoretical_mean);
    printf("Theoretical variance: %lf\n", theoretical_variance);
    
    //статистика хи-квадрат
    double chi_square = 0;

    //вывод частот и расчет статистики хи-квадрат
    printf("\nIntervals, observed and expected frequencies:\n");
    printf("Interval\t\tObserved\tExpected\n");
    for (int i = 0; i < k; i++) {
        double expected = N * (F(boundaries[i + 1]) - F(boundaries[i]));
        double diff = frequencies[i] - expected;
        chi_square += diff * diff / expected;

        printf("[%lf, %lf)\t%d\t\t%lf\n", boundaries[i],
                        boundaries[i + 1], frequencies[i], expected);
    }

    printf("Chi-square statistic: %lf\n", chi_square);

    // Критические значения хи-квадрат для alpha = 0.05 и df от 1 до 30
    double critical_values[] = {
        3.84, 5.99, 7.81, 9.49, 11.07, 12.59, 14.07, 15.51, 16.92, 18.31,
        19.68, 21.03, 22.36, 23.68, 24.99, 26.30, 27.59, 28.87, 30.14, 31.41,
        32.67, 33.92, 35.17, 36.42, 37.65, 38.89, 40.11, 41.34, 42.56, 43.77
    };

    int df = k - 1; // степени свободы
    if (df < 1 || df > 30) {
        printf("Критическое значение для степени свободы=%d не предусмотрено.\n", df);
        return 1;
    }

    double chi_crit = critical_values[df - 1];
    if (chi_square < chi_crit)
        printf("Гипотеза НЕ отвергается (x^2 = %.3f < %.3f) на уровне значимости 0.05\n", chi_square, chi_crit);
    else
        printf("Гипотеза отвергается (x^2 = %.3f >= %.3f) на уровне значимости 0.05\n", chi_square, chi_crit);

    //освобождение памяти
    free(boundaries);
    free(frequencies);

    return 0;
}