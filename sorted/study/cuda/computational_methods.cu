#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#define n 2
#define t0 0.0
#define tmax 10.0
#define tau 0.001
#define amountMethods 5

const int steps = (tmax - t0) / tau;

__device__ double f1(double* y, double t) {
	return y[1];
}

__device__ double f2(double* y, double t) {
	return -0.19 * t * y[1] - t * t * y[0];
}

__device__ double (*fs[n])(double*, double) = { f1, f2 };

__device__ void KernelEuler(double* y, int k, double t) {
	int i = threadIdx.x;
	y[(k + 1) * n + i] = y[k * n + i] + tau * fs[i](y + k * n, t);
	__syncthreads();
}

__device__ void KernelRungeKuttaTwo(double* y, int k, double t) {
	__shared__ double yy[n], ff[n];
	for (int i = 0; i < n; ++i) {
		yy[i] = y[k * n + i] + 0.5 * tau * fs[i](y + k * n, t);
		ff[i] = fs[i](yy, t + 0.5 * tau);
		y[(k + 1) * n + i] = y[k * n + i] + tau * ff[i];
	}
}

__device__ void KernelForecastCorrection(double* y, int k, double t) {
	__shared__ double ff[n], ffk[n];
	for (int i = 0; i < n; ++i) {
		ffk[i] = fs[i](y + k * n, t);
		y[(k + 1) * n + i] = y[k * n + i] + tau * fs[i](y + k * n, t);
		ff[i] = fs[i](y + (k + 1) * n + i, t + tau);
		y[k * n + i] += tau * (ff[i] + ffk[i]) / 2.0;
	}
}

__device__ void KernelRungeKuttaFour(double* y, int k, double t) {
	const int amountCoefs = 4;
	__shared__ double yy[n], r[n][amountCoefs];
	for (int i = 0; i < n; ++i) {
		r[i][0] = tau * fs[i](y + k * n, t);
		yy[i] = y[k * n + i] + 0.5 * r[i][0];
		r[i][1] = tau * fs[i](yy, t + 0.5 * tau);
		yy[i] = y[k * n + i] + 0.5 * r[i][1];
		r[i][2] = tau * fs[i](yy, t + 0.5 * tau);
		yy[i] = y[k * n + i] + 0.5 * r[i][2];
		r[i][3] = tau * fs[i](yy, t + 0.5 * tau);
		yy[i] = y[k * n + i] + 0.5 * r[i][3];
		y[(k + 1) * n + i] += (r[i][0] + 2 * r[i][1] + 2 * r[i][2] + r[i][3]) / 6.0;
	}
}

__device__ double partialDerivative(double (*f)(double*, double), double* y, double t, int j) {
	__shared__ double yy[n];
	double h = 0.001;
	for (int i = 0; i < n; ++i) yy[i] = y[i];
	yy[j] += h;
	return (f(yy, t) - f(y, t)) / h;
}

__device__ void KernelImplicitEuler(double* y, int k, double t) {
	double p[n] = {}, a[n][n] = {}, f[n] = {};
	double determinants[n + 1] = {};
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j) {
			a[i][j] = partialDerivative(fs[i], y, t, j);
			if (i == j) a[i][j] -= 1 / tau;
		}
	for (int i = 0; i < n; ++i) f[i] = -fs[i](y + k * n, t);
	determinants[0] = a[0][0] * a[1][1] - a[0][1] * a[1][0];
	determinants[1] = a[1][1] * f[0] - f[1] * a[0][1];
	determinants[2] = a[0][0] * f[1] - f[0] * a[1][0];
	for (int i = 0; i < n; ++i) {
		p[i] = determinants[i + 1] / determinants[0];
		y[(k + 1) * n + i] = y[k * n + i] + p[i];
	}
}

__device__ void (*methods[amountMethods])(double*, int, double) = { KernelEuler, KernelRungeKuttaTwo,
		KernelForecastCorrection, KernelRungeKuttaFour, KernelImplicitEuler };

__global__ void Compute(int m, double* ydev) {
	for (int k = 0; k <= steps; ++k) {
		double t = k * tau + t0;
		methods[m](ydev, k, t);
	}
}

char* concat(const char* s1, const char* s2)
{
	const size_t len1 = strlen(s1);
	const size_t len2 = strlen(s2);
	char* result = (char*)malloc(len1 + len2 + 1);
	memcpy(result, s1, len1);
	memcpy(result + len1, s2, len2 + 1); 
	return result;
}

int main() {
	char* fileExt = ".csv";
	char* titlesMethods[amountMethods] = { "Explicit Euler", "Runge-Kutta 2",
		"Forecast-Correction", "Runge-Kutta 4", "Implicit Euler" };

	int size = n * (steps + 1) * sizeof(double);
	double* y = (double*)malloc(size);
	y[0] = 0.0;
	y[1] = 0.05;
	float time;

	double* yDev = nullptr;
	cudaMalloc((void**)&yDev, size);

	cudaMemcpy(yDev, y, n * sizeof(double), cudaMemcpyHostToDevice);

	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	for (int m = 0; m < amountMethods; ++m) {
		double averageTime = 0.0;
		int amountIters = 5;

		printf("[%s Method]\n", titlesMethods[m]);

		for (int i = 0; i < amountIters; ++i) {
			cudaEventRecord(start, NULL);

			Compute << <1, n >> > (m, yDev);

			cudaEventRecord(stop, NULL);
			cudaEventSynchronize(stop);
			cudaEventElapsedTime(&time, start, stop);
			cudaMemcpy(y, yDev, size, cudaMemcpyDeviceToHost);
			printf("| Iter #%d Done <%fs> |\n", i, time / 1000.0);
			averageTime += time / 1000.0;
		}
		printf("* Average time: <%fs> *\n\n", averageTime / amountIters);
		FILE* f = fopen(concat(titlesMethods[m], fileExt), "w");
		for (int k = 0; k <= steps; ++k) {
			fprintf(f, "%f, %f, %f\n", k * tau + t0, y[k * n], y[k * n + 1]);
		}
		fflush(f);
		fclose(f);
	}	
	
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	cudaFree(yDev);
	free(y);
}
