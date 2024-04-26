#include <iostream>
#include <iomanip>
#include <chrono>
#include <fstream>

const int n = 2;
const double t0 = 0.0;
const double tmax = 10.0;
const double tau = 0.001;

double f1(double* y, double t) {
    return y[1];
}

double f2(double* y, double t) {
    return -0.19 * t * y[1] - t * t * y[0];
}

double (*fs[n])(double*, double) = {f1, f2};

void ExplicitEuler(double* y, double t, int k) {
    for (int i = 0; i < n; ++i) {
        y[(k + 1) * n + i] = y[k * n + i] + tau * fs[i](y + k * n, t);
    }
}

void RungeKuttaTwo(double* y, double t, int k) {
    double yy[n] = {}, ff[n] = {};
    for (int i = 0; i < n; ++i) {
        yy[i] = y[k * n + i] + 0.5 * tau * fs[i](y + k * n, t);
    }
    for (int i = 0; i < n; ++i) {
        ff[i] = fs[i](yy, t + 0.5 * tau);
    }
    for (int i = 0; i < n; ++i) {
        y[(k + 1) * n + i] = y[k * n + i] + tau * ff[i];
    }
}

void ForecastCorrection(double* y, double t, int k) {
    double ff[n] = {}, ffk[n] = {};
    for (int i = 0; i < n; ++i) {
        ffk[i] = fs[i](y + k * n, t);
    }
    for (int i = 0; i < n; ++i) {
        y[(k + 1) * n + i] = y[k * n + i] + tau * fs[i](y + k * n, t);
    }
    for (int i = 0; i < n; ++i) {
        ff[i] = fs[i](y + (k + 1) * n + i, t + tau);
    }
    for (int i = 0; i < n; ++i) {
        y[k * n + i] += tau * (ff[i] + ffk[i]) / 2.0;
    }
}

void RungeKuttaFour(double* y, double t, int k) {
    const int amountCoefs = 4;
    double yy[n] = {}, r[n][amountCoefs];
    for (int i = 0; i < n; ++i) r[i][0] = tau * fs[i](y + k * n, t);
    for (int i = 0; i < n; ++i) yy[i] = y[k * n + i] + 0.5 * r[i][0];
    for (int i = 0; i < n; ++i) r[i][1] = tau * fs[i](yy, t + 0.5 * tau);
    for (int i = 0; i < n; ++i) yy[i] = y[k * n + i] + 0.5 * r[i][1];
    for (int i = 0; i < n; ++i) r[i][2] = tau * fs[i](yy, t + 0.5 * tau);
    for (int i = 0; i < n; ++i) yy[i] = y[k * n + i] + 0.5 * r[i][2];
    for (int i = 0; i < n; ++i) r[i][3] = tau * fs[i](yy, t + 0.5 * tau);
    for (int i = 0; i < n; ++i) yy[i] = y[k * n + i] + 0.5 * r[i][3];
    for (int i = 0; i < n; ++i) y[(k + 1) * n + i] += (r[i][0] + 2 * r[i][1] + 2 * r[i][2] + r[i][3]) / 6.0;
}

double partialDerivative(double (*f)(double*, double), double* y, double t, int j) {
    double yy[n] = {}, h = 0.001;
    for (int i = 0; i < n; ++i) yy[i] = y[i];
    yy[j] += h;
    return (f(yy, t) - f(y, t)) / h;
}

void ImplicitEuler(double* y, double t, int k) {
    double p[n] = {}, a[n][n] = {}, f[n] = {};
    double determinants[n + 1] = {};
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            a[i][j] = partialDerivative(fs[i], y, t, j);
            if (i == j) a[i][j] -= 1/ tau;
        }
    for (int i = 0; i < n; ++i) f[i] = -fs[i](y + k * n, t);
    determinants[0] = a[0][0] * a[1][1] - a[0][1] * a[1][0];
    determinants[1] = a[1][1] * f[0] - f[1] * a[0][1];
    determinants[2] = a[0][0] * f[1] - f[0] * a[1][0];
    for (int i = 0; i < n; ++i) p[i] = determinants[i + 1] / determinants[0];
    for (int i = 0; i < n; ++i) y[(k + 1) * n + i] = y[k * n + i] + p[i];
}

int main() {
    bool areResultsPrintedToConsole = false;
    bool areResultsPrintedToFile = false;
    std::string fileExt = ".csv";

    const int amountMethods = 5;
    void (*methods[amountMethods])(double*, double, int) = {ExplicitEuler, RungeKuttaTwo, ForecastCorrection,
                                                            RungeKuttaFour, ImplicitEuler};
    std::string titlesMethods[amountMethods] = {"Explicit Euler", "Runge-Kutta 2", "Forecast-Correction",
                                                "Runge-Kutta 4", "Implicit Euler"};

    int steps = int((tmax - t0) / tau + 1);
    int size = n * steps * sizeof(double);
    auto* y = new double[size] {0.0, 0.05};

    for (int mn = 0; mn < amountMethods; ++mn) {
        std::cout<<titlesMethods[mn] + " Method"<<std::endl;
        double averageTime = 0.0;
        int amountIters = 5;
        for (int i= 0; i < amountIters; ++i) {
            std::cout<<"Iteration #" << i << " - ";
            auto start = std::chrono::high_resolution_clock::now();
            for (int k = 0; k < steps - 1; ++k) {
                double t = k * tau + t0;
                methods[mn](y, t, k);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> delta = end - start;
            averageTime += delta.count();
            std::cout<<std::fixed << std::setprecision(7) << "Done for " << averageTime<<"s"<<std::endl;
        }
        std::cout << "Average time: " << averageTime/amountIters<<"s"<<std::endl;

        if (areResultsPrintedToConsole) {
            std::cout<<"Results:"<<std::endl;
            for (int k = 0; k < steps; ++k) {
                std::cout << std::fixed << std::setprecision(2)<< "Time="<<k * tau + t0;
                std::cout<<std::fixed << std::setprecision(10)<<" "<<"y[0]="<<y[k * n]<<" "<<"y[1]="<<y[k * n + 1]<<std::endl;
            }
        }

        if (areResultsPrintedToFile) {
            std::ofstream f0(titlesMethods[mn] + "0" + fileExt);
            std::ofstream f1(titlesMethods[mn] + "1" + fileExt);
            for (int k = 0; k < steps; ++k) {
                f0 << std::fixed << std::setprecision(2) << k * tau + t0<<",";
                f0 << std::fixed << std::setprecision(10)<<y[k * n]<<std::endl;
                f1 << std::fixed << std::setprecision(2) << k * tau + t0<<",";
                f1 << std::fixed << std::setprecision(10)<<y[k * n + 1]<<std::endl;
            }
        }

        std::cout<<std::endl;
    }
    delete[] y;
}
