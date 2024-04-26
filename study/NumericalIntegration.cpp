#include <cmath>
#include <iostream>
#include <chrono>
#include <omp.h>

const double h = 0.00001;

double f(double x) { return pow((x * log(x)), 2); }

double midpoint(int x, int n, bool isParallel) {
    double result = 0.0;
#pragma omp parallel for schedule(dynamic) reduction(+:result) if (isParallel)
    for (int i = 1; i <= n; ++i)
        result += h * f(x + i * h - h / 2);
    return result;
}

double trapezoid(int x, int n, bool isParallel) {
    double result = 0.0;
#pragma omp parallel for schedule(dynamic) reduction(+:result) if (isParallel)
    for (int i = 0; i < n; ++i)
        result += h * (f(x + i * h) + f(x + i * h + h)) / 2;
    return result;
}

double simpson(int x, int n, bool isParallel) {
    double result = 0.0;
#pragma omp parallel for schedule(dynamic) reduction(+:result) if (isParallel)
    for (int i = 0; i < n; ++i)
        result += h * (f(x + i * h) + 4 * f(x + h * (i + 0.5)) + f(x + i * h + h)) / 6;
    return result;
}

constexpr int METHODS_AMOUNT = 3;
const std::string METHODS_TITLES[METHODS_AMOUNT] = { "Midpoint", "Trapezoid", "Simpson" };
double (*methods[METHODS_AMOUNT])(int, int, bool) = { midpoint, trapezoid, simpson };

int main() {
    double a = 2.0, b = 3.0;
    int n = (b - a) / h;
    return 0;
    for (int m = 0; m < METHODS_AMOUNT; ++m)
        for (int p = 0; p < 2; ++p) {
            auto start = std::chrono::high_resolution_clock::now();
            double result = methods[m](a, n, p);
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> delta = end - start;
            std::cout << METHODS_TITLES[m] << " method ";
            if (p) std::cout << "parallel ";
            std::cout << "= " << result << " (";
            std::cout << delta.count() << "s)" << std::endl;
        }
}
