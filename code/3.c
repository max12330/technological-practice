#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <locale.h>


double rand_uniform() {
    return (double)rand() / RAND_MAX;
}


int main() {
    setlocale(LC_ALL, "Russian_Russia.65001");

    // 1. Константа C
    double C = 1.0 / log(1024.0 / 729.0);
    double M = C / 2.0;

    // 2. Ввод объёма выборки
    int N;
    printf("Введите количество генерируемых пар N: ");
    if (scanf("%d", &N) != 1 || N <= 0) {
        printf("Ошибка: N должно быть положительным целым числом.\n");
        return 1;
    }

    // 3. Инициализация генератора случайных чисел
    srand((unsigned int)time(NULL));

    // 4. Переменные для накопления статистик
    double sum_xi = 0.0, sum_xi2 = 0.0;
    double sum_eta = 0.0, sum_eta2 = 0.0;
    int accepted = 0;      // количество принятых точек
    int attempts = 0;      // общее число попыток
    double sum_xy = 0.0;


    // 5. Основной цикл моделирования (метод исключения)
    while (accepted < N) {
        // Генерируем равномерную точку в квадрате [1,2] x [1,2]
        double x = 1.0 + rand_uniform();   // x ~ U(1,2)
        double y = 1.0 + rand_uniform();   // y ~ U(1,2)

        // Генерируем высоту u ~ U(0, M)
        double u = M * rand_uniform();

        // Проверяем условие принятия: u < f(x,y) = C/(x+y)
        if (u < C / (x + y)) {
            // Принимаем пару
            sum_xi += x;
            sum_xi2 += x * x;
            sum_eta += y;
            sum_eta2 += y * y;
            sum_xy += x * y;
            accepted++;
        }
        attempts++;
    }

    // 6. Выборочные характеристики
    double sample_mean_xi = sum_xi / N;
    double sample_var_xi  = sum_xi2 / N - sample_mean_xi * sample_mean_xi;
    double sample_mean_eta = sum_eta / N;
    double sample_var_eta  = sum_eta2 / N - sample_mean_eta * sample_mean_eta;
    double sample_cov = sum_xy / N - sample_mean_xi * sample_mean_eta;
    double sample_corr = sample_cov / 
            (sqrt(sample_var_xi) * sqrt(sample_var_eta));


    // 7. Теоретические значения (выведены аналитически)
    //    C = 1 / (10*ln2 - 6*ln3)
    //    E[xi] = C/2
    //    E[xi^2] = C * ( (34/3)ln2 - 6ln3 - 1/2 )
    double ln2 = log(2.0);
    double ln3 = log(3.0);
    double denom = 10.0 * ln2 - 6.0 * ln3;   // = ln(1024/729)
    double C_theor = 1.0 / denom;

    double theor_mean = C_theor / 2.0;
    double theor_E2 = C_theor * ( (34.0/3.0) * ln2 - 6.0 * ln3 - 0.5 );
    double theor_var = theor_E2 - theor_mean * theor_mean;

    double theor_cov = C * (2.0 - (34.0/3.0)*ln2 + 6.0*ln3) - C*C/4.0;
    double theor_corr = theor_cov / theor_var; // т.к. дисперсии равны


    // 8. Вывод результатов
    printf("\n=== РЕЗУЛЬТАТЫ МОДЕЛИРОВАНИЯ ===\n");
    printf("Объём выборки N = %d\n", N);
    printf("Принято точек: %d, всего попыток: %d\n", accepted, attempts);
    printf("Эмпир. вер-ть принятия: %.4f (теор. 1/M = %.4f)\n",
           (double)accepted / attempts, 1.0 / M);

    printf("\n--- Координата xi ---\n");
    printf("Выборочное среднее:      %12.8f\n", sample_mean_xi);
    printf("Теоретическое среднее:   %12.8f\n", theor_mean);
    printf("Выборочная дисперсия:    %12.8f\n", sample_var_xi);
    printf("Теоретическая дисперсия: %12.8f\n", theor_var);

    printf("\n--- Координата eta ---\n");
    printf("Выборочное среднее:      %12.8f\n", sample_mean_eta);
    printf("Теор. среднее:   %12.8f\n", theor_mean);
    printf("Выборочная дисперсия:    %12.8f\n", sample_var_eta);
    printf("Теор. дисперсия: %12.8f\n", theor_var);

    printf("\n--- Ковариация и корреляция ---\n");
    printf("Выборочная ковариация:      %12.8f\n", sample_cov);
    printf("Теоретическая ковариация:   %12.8f\n", theor_cov);
    printf("Выборочный коэффициент корреляции: %12.8f\n", sample_corr);
    printf("Теоретический коэффициент корреляции: %12.8f\n", theor_corr);


    return 0;
}