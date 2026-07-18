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
    double theoretical_mean = 0;
    double theoretical_variance = 0;
    


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



    free(sample);

    return 0;
}