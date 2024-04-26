/* Вариант 8: двухфазная естественная сортировка с 5 вспомогательными файлами */

#include <array>
#include <fstream>
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>

uint_fast32_t externalNaturalTwoPhaseSort(const std::string &filename) {
    /* Sorts the specified file */
    uint_fast32_t amountOperations = 0;

    /* Sort cycle. Break if zero helper is not empty and others are empty */
    bool sortDone = false;
    while (true) {
        const int M = 5;
        /* Split source file to helpers */
        {
            /* Open helpers */
            std::array<std::ofstream, M> helpers{};
            for (int i = 0; i < M; ++i) {
                auto helper = (std::stringstream() << i << ".txt").str();
                helpers[i] = std::ofstream(helper);
                if (!helpers[i].is_open())
                    throw std::runtime_error((std::stringstream() << "Can't open file " << helper).str());
            }

            /* Write series to helpers */
            auto source = std::ifstream(filename);
            int currentFileNumber = 0;
            int v;
            int prev;
            while (source >> v) {
                if (prev > v) currentFileNumber = (currentFileNumber + 1) % M;

                helpers[currentFileNumber] << v << " ";
                prev = v;
            }

            for (int i = 0; i < M; ++i) {
                helpers[i].flush();
                helpers[i].close();
            }
            source.close();
        }

        /* Merge */
        {
            /* Open helpers to read data */
            std::array<std::ifstream, M> helpers{};
            for (int i = 0; i < M; ++i) {
                auto helper = (std::stringstream() << i << ".txt").str();
                helpers[i] = std::ifstream(helper);
                if (!helpers[i].is_open())
                    throw std::runtime_error((std::stringstream() << "Can't open file " << helper).str());
            }

            auto source = std::ofstream(filename);

            std::array<int, M> values{};

            int countReadFiles = 0;
            for (int i = 0; i < M; ++i)
                if (helpers[i] >> values[i])
                    ++countReadFiles;

            if (countReadFiles == 1)
                sortDone = true;

            /* Read records */
            while (true) {
                /* Read current record */
                std::array<bool, M> done{false};

                bool allFilesDone = true;
                for (const auto &f: helpers)
                    if (!f.eof())
                        allFilesDone = false;

                if (allFilesDone)
                    break;

                while (true) {
                    int min_i = -1, min_v = INT_MAX;
                    for (int i = 0; i < M; ++i) {
                        if (done[i] || helpers[i].eof())
                            continue;

                        int v = values[i];
                        if (v < min_v) {
                            min_v = v;
                            min_i = i;
                        }
                    }

                    if (min_i < 0)
                        break;

                    source << min_v << " ";
                    if (!(helpers[min_i] >> values[min_i]) || min_v > values[min_i])
                        done[min_i] = true;
                }
            }

            for (int i = 0; i < M; ++i)
                helpers[i].close();
            source.flush();
            source.close();
        }

        if(sortDone)
            break;
        ++amountOperations;
    }

    return amountOperations;
}

int main() {
    /* Fills an array with numbers from low to up */
    std::random_device os_seed;
    const uint_fast32_t seed = os_seed();

    const uint_fast32_t LOW = 10, UP = 10000;
    std::mt19937 generator(seed);
    std::uniform_int_distribution<int32_t> distribute(LOW, UP);

    auto results = std::ofstream("results.csv");
    if (!results.is_open())
        throw std::runtime_error("Can't open file to write results");

    const uint_fast32_t MIN_SIZE = 10, MAX_SIZE = 1000;
    for (auto s = MIN_SIZE; s <= MAX_SIZE; ++s) {
        const std::string filename = "source.txt";
        auto source = std::ofstream(filename);
        if (!source.is_open())
            throw std::runtime_error("Can't open source file to write data");

        for (int i = 0; i < s; ++i)
            source << distribute(generator) << " ";

        source.flush();
        source.close();

        auto begin = std::chrono::high_resolution_clock::now();
        auto operationsCount = externalNaturalTwoPhaseSort(filename);
        auto end = std::chrono::high_resolution_clock::now();

        auto delta = std::chrono::duration<double>(end - begin);
        std::cout << std::fixed << std::setprecision(4) << s << " - " << delta.count() * 1000 << " ms" << std::endl;

        results << s << "," << operationsCount << std::endl;
    }

    results.flush();
    results.close();
}