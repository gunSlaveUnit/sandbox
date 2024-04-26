#include <fstream>
#include <iostream>
#include <chrono>
#include <limits>
#include <cmath>

using namespace std;

const int FILENAME_LENGTH = 30;

const int METHODS_AMOUNT = 3;
const char* METHODS_TITLES[METHODS_AMOUNT] = {"yacobi", "gauss-seidel", "gauss"};

const double e = 0.01;

void generateMatrix(int N) {
    char filenameA[256];
    char filenameB[256];

    sprintf_s(filenameA, "a%d.txt", N);
    sprintf_s(filenameB, "b%d.txt", N);

    ofstream matrixA(filenameA);
    ofstream matrixB(filenameB);

    if (!matrixA.is_open() || !matrixB.is_open()) {
        cout << "Can't open file to write matrix"<< endl;
        return;
    }

    auto* matrA = new double[N];
    auto* matrB = new double[N];

    for (int i = 0; i < N; i++) {
        double sum = 0.0;
        for (int j = 0; j < N; j++)
            sum += matrA[j] = rand() % 100;
        matrA[i] = sum + 1;

        for (int j = 0; j < N; j++)
            matrixA<< matrA[j]<< '\t';
        matrixA<< endl;

        matrB[i] = rand() % 100;
        matrixB<< matrB[i]<< '\t';
    }

    matrixA.close();
    matrixB.close();

    delete[] matrA;
    delete[] matrB;
}

void yacobi(double** a, double* b, int n) {
    char xFilename[FILENAME_LENGTH];
    sprintf_s(xFilename, FILENAME_LENGTH, "x%d%s.txt", n, METHODS_TITLES[0]);

    auto* x = new double[n];
    auto* xx = new double[n];

    std::ofstream xf(xFilename, std::ios_base::out);
    if (!xf.is_open()) return;

    for (int i = 0; i < n; ++i) x[i] = xx[i] = 1.0;

    double xmax;
    auto start = std::chrono::high_resolution_clock::now();
    do {
        xmax = -DBL_MAX;
        for (int i = 0; i < n; ++i) {
            double sum = 0.0;
            for (int j = 0; j < n; ++j)
                if (i != j) sum += a[i][j] * x[j];
            xx[i] = 1.0 / a[i][i] * (b[i] - sum);
            double absDiff = fabs(xx[i] - x[i]);
            if (absDiff > xmax) xmax = absDiff;
        }
        for (int i = 0; i < n; ++i) x[i] = xx[i];
    } while (xmax > e);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> delta = end - start;

    std::cout<<METHODS_TITLES[0]<<n<<" done for "<<delta.count()<<"s"<<std::endl;

    for (int i = 0; i < n; ++i) xf<< x[i] << " ";

    xf.flush();
    xf.close();

    delete[] x;
    delete[] xx;
}

void gauss_seidel(double** a, double* b, int n) {
    char xFilename[FILENAME_LENGTH];
    sprintf_s(xFilename, FILENAME_LENGTH, "x%d%s.txt", n, METHODS_TITLES[1]);

    auto* x = new double[n];

    std::ofstream xf(xFilename, std::ios_base::out);
    if (!xf.is_open()) return;

    for (int i = 0; i < n; ++i) x[i] = 1.0;

    double xmax, xx;
    auto start = std::chrono::high_resolution_clock::now();
    do {
        xmax = -DBL_MAX;
        for (int i = 0; i < n; ++i) {
            xx = x[i];
            double sum = 0.0;
            for (int j = 0; j < n; ++j)
                if (i != j) sum += a[i][j] * x[j];
            x[i] = 1.0 / a[i][i] * (b[i] - sum);
            double absDiff = fabs(x[i] - xx);
            if (absDiff > xmax) xmax = absDiff;
        }
    } while (xmax > e);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> delta = end - start;

    std::cout<<METHODS_TITLES[1]<<n<<" done for "<<delta.count()<<"s"<<std::endl;

    for (int i = 0; i < n; ++i) xf<< x[i] << " ";

    xf.flush();
    xf.close();

    delete[] x;
}

void gauss(double** a, double* b, int n) {
    char xFilename[FILENAME_LENGTH];
    sprintf_s(xFilename, FILENAME_LENGTH, "x%d%s.txt", n, METHODS_TITLES[2]);

    auto* x = new double[n];
    auto* w = new double[n];

    for (int i = 0; i < n; ++i) x[i] = 1.0;

    std::ofstream xf(xFilename, std::ios_base::out);
    if (!xf.is_open()) return;

    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 1; i < n; ++i) {
        int l = i - 1;
        for (int j = 0; j < n - i; ++j) w[j] = -a[j+i][l] / a[l][l];
        for (int k = i; k < n; ++k) {
            for (int j = 0; j < n; ++j) a[k][j] += a[l][j] * w[k-i];
            b[k] += b[l] * w[k-i];
        }
    }

    double sum;
    for(int j = n - 1; j >= 0; --j) {
        sum = 0;
        for (int i = j; i < n - 1; ++i) sum += a[j][i+1] * x[i+1];
        x[j] = (b[j] - sum) / a[j][j];
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> delta = end - start;

    std::cout<<METHODS_TITLES[2]<<n<<" done for "<<delta.count()<<"s"<<std::endl;

    for (int i = 0; i < n; ++i) xf<< x[i] << " ";

    xf.flush();
    xf.close();

    delete[] w;
    delete[] x;
}

void (*methods[METHODS_AMOUNT])(double**, double*, int) = {yacobi, gauss_seidel, gauss};

int main() {
    srand(time(0));
    const int SIZES_AMOUNT = 3;
    int ns[SIZES_AMOUNT] = {100, 250, 500};

    char aFilename[FILENAME_LENGTH];
    char bFilename[FILENAME_LENGTH];

    for (int n : ns) generateMatrix(n);
    for (auto& method : methods)
        for (int n : ns) {
            sprintf_s(aFilename, FILENAME_LENGTH, "a%d.txt", n);
            sprintf_s(bFilename, FILENAME_LENGTH, "b%d.txt", n);

            auto** a = new double* [n];
            for (int i = 0; i < n; ++i) a[i] = new double[n];
            auto* b = new double[n];

            std::ifstream af(aFilename, std::ios_base::in);
            std::ifstream bf(bFilename, std::ios_base::in);
            if (!af.is_open() && !bf.is_open()) return -1;

            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) af >> a[i][j];
                bf >> b[i];
            }

            method(a, b, n);

            af.close();
            bf.close();

            for (int i = 0; i < n; ++i) delete[] a[i];
            delete[] a;
            delete[] b;
        }
}
