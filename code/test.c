#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

static double rand01(void) {
    return (double)rand() / (double)RAND_MAX;
}

int main(void) {
    const int samples = 10000;
    const double C = 1.0 / log(1024.0 / 729.0);
    const double M = C / 2.0;

    srand((unsigned)time(NULL));

    int accepted = 0;
    for (int i = 0; i < samples; ++i) {
        while (1) {
            double x = 1.0 + rand01();
            double y = 1.0 + rand01();
            double u = rand01();

            double target_density = C / (x + y);
            double acceptance = target_density / M;

            if (u < acceptance) {
                printf("%.8f %.8f\n", x, y);
                accepted++;
                break;
            }
        }
    }

    fprintf(stderr, "Accepted: %d out of %d attempts\n", accepted, samples);
    return 0;
}
