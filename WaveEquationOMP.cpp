#include <cstdio>
#include <memory>
#include <cstdlib>
#include <omp.h>

int main() {
	const float xmax = 1.5, ymax = 2.0, 
		x = 1.0, y = 1.3, v = 2.4, 
		h = 0.02, t0 = 0.0, tmax = 1.0, tau = 0.01, c = 1.0;

	float r = c * c * tau * tau / (h * h);

	const int n = (int)(xmax / h) + 1,
		m = (int)(ymax / h) + 1,
		size = n * m * sizeof(float);

	float* u = (float*)malloc(size); 
	float* uold = (float*)malloc(size); 
	float* unew = (float*)malloc(size); 

	memset(u, 0, size);
	memset(uold, 0, size);
	memset(unew, 0, size);

	int i = x / h, j = y / h;
	uold[i * m + j] = - tau * v;

	float t = t0;
	double start = omp_get_wtime();
	do
	{
#pragma omp parallel for private (j)
		for (i = 1; i < n - 1; i++)
			for (j = 1; j < m - 1; j++)
				unew[i * m + j] = 2.0f * u[i * m + j] 
				- uold[i * m + j] 
				+ r * (
					u[(i - 1) * m + j] 
					+ u[(i + 1) * m + j] 
					+ u[i * m + j - 1] 
					+ u[i * m + j + 1] 
					- 4.0f * u[i * m + j]
					);

#pragma omp parallel for private (j)
		for (i = 1; i < n - 1; ++i)
			for (j = 1; j < m - 1; ++j) {
				uold[i * m + j] = u[i * m + j];
				u[i * m + j] = unew[i * m + j];
			}
		t += tau;
	} while (t < tmax);
	double stop = omp_get_wtime();

	FILE* f;
	fopen_s(&f, "final.txt", "w");
	if (f) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++)
				fprintf(f, "%8.5f\t ", u[i * m + j]);
			fprintf(f, "\n");
		}
		fclose(f);
	}

	printf("Done for %fs\n", stop - start);

	free(u);
	free(uold);
	free(unew);
}