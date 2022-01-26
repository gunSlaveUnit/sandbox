#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#define n 1000
#define t0 0.0
#define tmax 10.0
#define tau 0.01
#define steps (tmax - t0) / tau
#define a1 1.0f
#define a2 2.0f
#define p1 3
#define p2 2
#define m 1
#define maxpos 5.0f
#define minpos -5.0f
#define blocksize 32
#define z 4

__device__ double sum(double* u, int i, int k, int f) {
	double sum = 0.0;
	for (int j = 0; j < n; ++j) {
		if (i == j) continue;
		double* ie = u + (k * n + i) * z;
		double* je = u + (k * n + j) * z;
		double xi = ie[2];
		double yi = ie[3];
		double xj = je[2];
		double yj = je[3];
		double dx = xj - xi;
		double dy = yj - yi;
		double d1 = a1 * (f ? dx : dy);
		double d2 = a2 * (f ? dx : dy);
		sum += d1 / pow(dx * dx + dy * dy, p1 / 2) -
			d2 / pow(dx * dx + dy * dy, p2 / 2);
	}
	return sum / m;
}

__global__ void Kernel(double* u) {
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i >= n) return;

	for (int k = 0; k < steps; ++k) {
		double* ce = u + (k * n + i) * z;
		double* ne = u + ((k + 1) * n + i) * z;
		ne[0] = ce[0] + tau * sum(u, i, k, 0); // vx
		ne[1] = ce[1] + tau * sum(u, i, k, 1); // vy
		ne[2] = ce[2] + tau * ce[0];           // x
		ne[3] = ce[3] + tau * ce[1];           // y

		if (!(minpos <= ne[2] && ne[2] <= maxpos)) { 
			ne[0] *= -1; 
			ne[2] += tau * ne[0];
		}
		if (!(minpos <= ne[3] && ne[3] <= maxpos)) { 
			ne[1] *= -1; 
			ne[3] += tau * ne[1];
		}

		__syncthreads();
	}
}

int main() {
	srand(time(0));
	int size = n * z * (steps + 1) * sizeof(double);
	double* u = (double*)malloc(size); // vx vy x y
	for (int i = 0; i < n; ++i) {
		u[i * z] = 0.0;
		u[i * z + 1] = 0.0;
		u[i * z + 2] = (double)rand() / RAND_MAX - 0.5;
		u[i * z + 3] = (double)rand() / RAND_MAX - 0.5;
	}

	double* udev = nullptr;
	cudaMalloc((void**)&udev, size);
	cudaMemcpy(udev, u, size, cudaMemcpyHostToDevice);

	float time;
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaEventRecord(start, NULL);
	Kernel << <(n / blocksize) + 1, blocksize >> > (udev);
	cudaEventRecord(stop, NULL);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&time, start, stop);

	cudaMemcpy(u, udev, size, cudaMemcpyDeviceToHost);

	std::ofstream f("results.txt");

	for (int k = 0; k <= steps; ++k) {
		for (int i = 0; i < n; ++i) {
			f << u[(k * n + i) * z + 2] << " ";
			f << u[(k * n + i) * z + 3] << std::endl;
		}
	}

	f.flush();
	f.close();

	std::cout << "Done for " << time / 1000.0 << "s";

	cudaFree(udev);
	free(u);
}
