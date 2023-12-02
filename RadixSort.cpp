#include <iostream>
#include <chrono>
#include <random>
#include <string>
#include <fstream>
#include <cstring>
#include <array>
#include <vector>

void print(const int32_t* a, const size_t& size) {
    /* Prints an array one line separated by a space */
    for (int i = 0; i < size; ++i)
        std::cout << a[i] << " ";
}

void fill(int32_t* const a, const size_t& size, const int32_t& low, const int32_t& up) {
    /* Fills an array with numbers from low to up */
    std::random_device os_seed;
    const int32_t seed = os_seed();

    std::mt19937 generator(seed);
    std::uniform_int_distribution<int32_t> distribute(low, up - 1);

    for (size_t i = 0; i < size; ++i)
        a[i] = distribute(generator);
}

uint32_t radixSort(int32_t* const a, const size_t& size) {
    uint32_t operationsAmount = 0;

    std::vector<uint32_t> is;
    for (size_t i = 0; i < size; ++i)
        is.push_back(i);

    std::vector<uint32_t> b;
    for(size_t i = 0; i < size; ++i)
        b.push_back(a[i]);

    uint32_t digitsAmount = 0;
    for (size_t i = 0; i < size; ++i) {
        std::string t = std::to_string(b[i]);
        uint32_t amount = t.length();
        if (amount > digitsAmount)
            digitsAmount = amount;
    }

    const uint32_t optionsNumber = 10;
    std::array<std::vector<uint32_t>, optionsNumber> field;

    for (uint32_t i = 0; i < digitsAmount; ++i) {
        for(auto& chunk : field)
            chunk.clear();

        for (size_t e = 0; e < size; ++e) {
            auto digitCutter = static_cast<uint32_t>(pow(10, i));
            uint32_t digit = (a[is[e]] / digitCutter) % 10;
            field[digit].push_back(is[e]);
            ++operationsAmount;
        }

        uint32_t place = 0;
        for(const auto& chunk : field)
            for(const auto& index : chunk) {
                is[place++] = index;
                ++operationsAmount;
            }
    }

    for (size_t i = 0; i < size; ++i)
        a[i] = b[is[i]];

    return operationsAmount;
}

int main() {
    const size_t MAX_SIZE = 5000;
    const int32_t LOW = 1000, UP = 10000;

    std::ofstream data("data.csv", std::ios::out);
    if (!data.is_open())
        throw std::runtime_error("Can't open file to write results");

    data << "Size" << "," << "Operations amount" << std::endl;

    for (size_t size = 10; size < MAX_SIZE; ++size) {
        auto* a = new int32_t[size];
        fill(a, size, LOW, UP);

        std::cout << std::endl << "Size: " << size << std::endl;
        std::cout << "Unsorted:" << std::endl;
        print(a, size);
        std::cout << std::endl;

        auto begin = std::chrono::high_resolution_clock::now();
        auto operationsAmount = radixSort(a, size);
        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Sorted:" << std::endl;
        print(a, size);
        std::cout << std::endl;

        auto delta = std::chrono::duration<double>(end - begin);
        std::cout << std::fixed << std::setprecision(6) << "Done for " << delta.count() * 1000 << " ms" << std::endl;
        std::cout << std::endl;

        data << size << "," << operationsAmount << std::endl;

        delete[] a;
    }

    data.flush();
    data.close();
}
