#include <cstdio>
#include <cstdlib>
#include <memory>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void Kernel(double* ud, double* oldud, double* newud, int n, int m, double r) {
	int i = blockIdx.x * blockDim.x + threadIdx.x + 1;
	int j = blockIdx.y * blockDim.y + threadIdx.y + 1;

	if (i >= n - 1 || j >= m - 1) return;

	newud[i * m + j] = 2.0 * ud[i * m + j] - oldud[i * m + j] +
		r * (ud[(i - 1) * m + j] +
			ud[(i + 1) * m + j] +
			ud[i * m + j - 1] +
			ud[i * m + j + 1] -
			4.0 * ud[i * m + j]);

	__syncthreads();

	oldud[i * m + j] = ud[i * m + j];
	ud[i * m + j] = newud[i * m + j];
}

int main() {
	const int BLOCKSIZE = 32;
	const double X = 1.0, Y = 1.3, v0 = 2.4,
		xmax = 1.5, ymax = 2.0, h = 0.02,
		t0 = 0.0, tmax = 1.0, tau = 0.01,
		c = 1.0;
	const int n = int((xmax / h) + 1), m = int((ymax / h) + 1);
	const double r = c * c * tau * tau / (h * h);

	int size = n * m * sizeof(double);

	double* oldu = (double*)malloc(size);
	double* u = (double*)malloc(size);

	int i = int(X / h), j = int(Y / h);
	memset(u, 0, size);
	memset(oldu, 0, size);
	oldu[i * m + j] = -tau * v0;

	double* oldud = nullptr;
	double* ud = nullptr;
	double* newud = nullptr;
	cudaMalloc((void**)&oldud, size);
	cudaMalloc((void**)&ud, size);
	cudaMalloc((void**)&newud, size);
	cudaMemcpy(oldud, oldu, size, cudaMemcpyHostToDevice);
	cudaMemcpy(ud, u, size, cudaMemcpyHostToDevice);

	float time;
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaEventRecord(start, NULL);
	for (float t = t0; t < tmax; t += tau) {
		Kernel << <dim3(n / BLOCKSIZE + 1, m / BLOCKSIZE + 1),
			dim3(BLOCKSIZE, BLOCKSIZE) >> > (ud, oldud, newud, n, m, r);
		cudaThreadSynchronize();
	}
	cudaEventRecord(stop, NULL);
	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&time, start, stop);

	cudaMemcpy(u, ud, size, cudaMemcpyDeviceToHost);

	FILE* f;
	fopen_s(&f, "FINAL.txt", "w");
	if (f) {
		for (i = 0; i < n; ++i) {
			for (j = 0; j < m; ++j)
				fprintf(f, "%8.10f\t ", u[i * m + j]);
			fprintf(f, "\n");
		}
		fclose(f);
	}

	printf("Done for %fs", time / 1000);

	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	cudaFree(oldud);
	cudaFree(ud);
	cudaFree(newud);
	free(oldu);
	free(u);
}
