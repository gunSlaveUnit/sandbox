#include <cstdlib>
#include <cstdio>
#include <omp.h>

double f(double x1, double x2) {
	const double a = 11.2, b = -0.4, c = 0.95, d = 0.21;
	return a * x1 + b * x2 + exp(c * x1 * x1 + d * x2 * x2);
}

const double t = 0.001;

double partialDerivative(double* x, int i) {
	double x1 = (!i) ? x[0] + t : x[0];
	double x2 = (i) ? x[1] + t : x[1];
	return (f(x1, x2) - f(x[0], x[1])) / t;
}

int main() {
	const double x1min = -5.0, x2min = -5.0, x1max = 5.0, x2max = 5.0, e = 0.0001;

	const int COORDS_AMOUNT = 2;
	double x[COORDS_AMOUNT] = {
		(x1min + x1max) / 2,
		(x2min + x2max) / 2,
	};

	double sum;
	double p[COORDS_AMOUNT];
	double start = omp_get_wtime();
	do {
		sum = 0;

				p[0] = partialDerivative(x, 0);
				x[0] -= t * p[0];
				sum += p[0] * p[0];
			
				p[1] = partialDerivative(x, 1);
				x[1] -= t * p[1];
				sum += p[1] * p[1];
	} while (sum >= e &&
		(x1min <= x[0] && x[0] <= x1max) &&
		(x2min <= x[1] && x[1] <= x2max));

	printf("Done for %fs\n", omp_get_wtime() - start);
	printf("x1 = %f x2 = %f res = %f", x[0], x[1], f(x[0], x[1]));
}
