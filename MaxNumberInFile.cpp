/*
* 9.1 Написать программу нахождения максимального числа в файле.
*/

#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>

#include "mpi.h"

int main(int argc, char **argv) {
    int rank, size, root = 0;

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS ||
        MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS ||
        MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS) {
        MPI_Finalize();
        return -1;
    }

    const uint_fast32_t BUFFER_SIZE = 30;
    std::vector<int> buffer(BUFFER_SIZE, INT_MIN); // буфер для чтения данных
    int globalMax = INT_MIN; // минимум - результат работы программы
    int n, link; // n - какой элемент буфера читаем, link - какому процессу отдаем прочитанный буфер

    if (rank == root) {
        auto begin = std::chrono::high_resolution_clock::now();

        // работа корневого процесса
        std::ifstream in("100.txt"); // открываем файл на чтение
        n = 0, link = 1;

        while (in >> buffer[n]) {
            // читаем элемент буфера
            ++n;

            if (n == BUFFER_SIZE) {
                // буфер заполнен
                MPI_Send(buffer.data(), BUFFER_SIZE, MPI_INT, link, 0, MPI_COMM_WORLD); // отправляем данные процессу
                ++link;
                n = 0;
                buffer.clear();
            }

            if (link == size)
                link = 1;
        }

        // выясняем максимум в остатке, который достался на обработку root-процессу
        globalMax = *std::max_element(buffer.begin(), buffer.end());

        // рассылаем всем не root процессам сигнал о завершении работы
        buffer[0] = -1;
        for (int i = 1; i < size; ++i)
            MPI_Send(buffer.data(), 1, MPI_INT, i, 0, MPI_COMM_WORLD);

        for (int i = 1; i < size; ++i) {
            // собираем со всех процессов максимумы, которые посчитали они
            int localMax;
            MPI_Recv(&localMax, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // сравниваем полученный локальный максимум с глобальным
            if (localMax > globalMax)
                globalMax = localMax;
        }

        // вывод результатов
        std::cout << "Max: " << globalMax << std::endl;
        auto end = std::chrono::high_resolution_clock::now();
        auto delta = std::chrono::duration<double>(end - begin);
        std::cout << std::fixed << "Elapsed time: " << delta.count() << " s" << std::endl;
    } else {
        // работа дочерних процессов
        while (true) {
            // получаем буфер с данными - участок, на котором будем искать локальный максимум
            MPI_Recv(buffer.data(), BUFFER_SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            // если поступил сигнал о том, что данных больше нет, завершаем работу
            if (buffer[0] == -1)
                break;

            // находим максимум
            // в данном случае globalMax - максимальный элемент среди всех участков, которые отдавались процессу
            // localMax - максимальный элемент на вновь поступившем участке
            auto localMax = *std::max_element(buffer.begin(), buffer.end());
            if (localMax > globalMax)
                globalMax = localMax;
        }

        // отсылаем найденный максимум среди всех участков, отданных на обработку данному процессу
        MPI_Send(&globalMax, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}