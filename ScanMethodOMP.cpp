#include <cmath>
#include <cstdio>
#include <omp.h>

double f(double x1, double x2) {
    const double a = 11.2, b = -0.4, c = 0.95, d = 0.21;
    return a * x1 + b * x2 + exp(c * x1 * x1 + d * x2 * x2);
}

void scan(double* p, double xmin, double ymin, double xmax, double ymax, double h) {
    int n = (xmax - xmin) / h, m = (ymax - ymin) / h;
    double gres = f(xmin, ymin);
#pragma omp parallel 
    {
        double res = f(xmin, ymin), lx, ly;
#pragma omp for 
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j) {
                double x = xmin + i * h, y = ymin + j * h;
                double v = f(x, y);
                if (v < res) {
                    res = v;
                    lx = x;
                    ly = y;
                }
            }

#pragma omp critical
        {
            if (res < gres) {
                gres = res;
                p[0] = lx;
                p[1] = ly;
            }
        }
    }
}

int main() {
    double p[] = { 0.0, 0.0 };
    double hs[] = { 0.1, 0.001 };

    double xmin = -5.0, xmax = 5.0, ymin = -0.5, ymax = 5.0;
    double time = 0.0;
    for (double h : hs) {
        double start = omp_get_wtime();
        scan(p, xmin, ymin, xmax, ymax, h);
        time += omp_get_wtime() - start;
        printf("h = %f\tx = %f\t y = %f\tres = %f\n", h, p[0], p[1], f(p[0], p[1]));

        xmin = (p[0] - h > xmin) ? p[0] - h : xmin;
        xmax = (p[0] + h < xmax) ? p[0] + h : xmax;

        ymin = (p[1] - h > ymin) ? p[1] - h : ymin;
        ymax = (p[1] + h < ymax) ? p[1] + h : ymax;
    }
    printf("Done for %fs", time);
}
