//
// Freezing problem
//

#include <vector>
#include <chrono>
#include <fstream>
#include <cstring>
#include <iostream>

#include <mpi.h>

int main() {
	MPI_Init(nullptr, nullptr);

	int size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	constexpr auto root = 0;

	constexpr double A_WATER = 8.2e-6;
	constexpr double A_ICE = 1.0e-6;
	constexpr double L_ICE = 15.5;
	constexpr double L_WATER = 4.19;
	constexpr double R_WATER = 1000.0;

	constexpr double T_ICE = -10.0;
	constexpr double T_WATER = 10.0;

	constexpr double H = 3.3e5;
	constexpr double TAU = 0.01;

	constexpr double L = 0.01;
	constexpr double h = 0.001;
	constexpr int N = (L / h) + 1;

	constexpr double A = TAU / (H * R_WATER * h);
	constexpr double A1 = (A_ICE * TAU) / (h * h);
	constexpr double A2 = (A_WATER * TAU) / (h * h);

	auto interval_width = N / size;
	auto interval_remainder = N % size;
	if (rank < interval_remainder)
    	++interval_width;

	std::vector<int> interval_widths(size);
	MPI_Allgather(
    &interval_width, 1, MPI_INT,
    	interval_widths.data(), 1, MPI_INT,
    	MPI_COMM_WORLD
	);

	std::vector<int> x_starts(size);
	x_starts[0] = 0;
	for (int i = 1; i < x_starts.size(); ++i)
    	x_starts[i] = interval_widths[i - 1] + x_starts[i - 1];
   
	auto w = interval_width;
	auto is_side_process = rank == 0 || rank == size - 1;
	auto added_points_amount = is_side_process ? 1 : 2;
	w += added_points_amount;
    
	std::vector<double> T_local(w, T_WATER);
	if (rank == root)
    	T_local[0] = T_ICE;

	std::vector<double> T_local_n(w, T_WATER);
	if (rank == root)
    	T_local_n[0] = T_ICE;

	auto e = 0.0;
	auto transition_point = 1;

	std::ofstream file("parallel.txt", std::ios::app);

	auto start_time = std::chrono::high_resolution_clock::now();
    
	auto is_print_enabled = false;
	do {
    if (is_print_enabled) {
    	for (auto i = 0; i < size; ++i) {
            	if (i == rank) {
                	auto start = rank == root ? 0 : 1;
                	auto end = rank == size - 1 ? w : w - 1;
           	 
                	for (auto j = start; j < end; ++j) {
                    	file << T_local[j] << ",";
                    	file.flush();
                	}
            	}

            	MPI_Barrier(MPI_COMM_WORLD);
        	}

        	if (rank == size - 1)
            	file << std::endl;
    }
  	 
    	for (auto i = 1; i < w - 1; ++i) {
        	auto global_i = x_starts[rank] + i - (rank != 0);

        	if (global_i < transition_point)
            	T_local_n[i] = T_local[i] + A1 * (T_local[i - 1] - 2.0 * T_local[i] + T_local[i + 1]);
        	else if (global_i > transition_point)
            	T_local_n[i] = T_local[i] + A2 * (T_local[i - 1] - 2.0 * T_local[i] + T_local[i + 1]);
        	else
            	T_local_n[i] = 0.0;
    	}

    	if (rank == size - 1)
        	T_local_n[w - 1] = T_local_n[w - 2];

    	memcpy(T_local.data(), T_local_n.data(), sizeof(double) * w);
 
    	if (rank & 1) {
        	MPI_Ssend(&T_local[1], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD);
        	MPI_Recv(&T_local[0], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
       	 
    	if (rank != size - 1) {
            	MPI_Ssend(&T_local[w - 2], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
            	MPI_Recv(&T_local[w - 1], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        	}
    	} else {
        	if (rank != size - 1) {
            	MPI_Recv(&T_local[w - 1], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            	MPI_Ssend(&T_local[w - 2], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
        	}
       	 
    	if (rank) {
            	MPI_Recv(&T_local[0], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            	MPI_Ssend(&T_local[1], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD);
    	}
    	}  	 
    
    auto start = x_starts[rank];
    auto end = x_starts[rank] + interval_width;
    auto is_transition_point_owner = (transition_point >= start && transition_point < end);
    
    if (is_transition_point_owner) {
        	e -= A * (
        	L_ICE * T_local[transition_point - x_starts[rank] - 1 + (rank != 0)]
        	+ L_WATER * T_local[transition_point - x_starts[rank] + 1 + (rank != 0)]
    	);

        	for (auto i = 0; i < size; ++i)
            	if (i != rank)
                	MPI_Send(&e, 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
    	} else
        	MPI_Recv(&e, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    	transition_point = std::max(static_cast<int>(e / h), 1);
	} while (e < L);

	auto end_time = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration<double>(end_time - start_time);
	std::cout << "Process " << rank << " done: " << duration.count() << "\n";

	file.close();

	MPI_Finalize();
}
