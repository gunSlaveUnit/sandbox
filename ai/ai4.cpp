#include <iostream>
#include <array>
#include <cstring>
#include <cmath>
#include <algorithm>

using i32 = int_fast32_t;
using byte = int_fast8_t;

const i32 LAYER_DIMENSION = 20;
const i32 LAYER_DIMENSION_THIRD = LAYER_DIMENSION / 3;
const i32 MINIMUM_RECOGNITION_THRESHOLD = 3;
const double LEARNING_WEIGHT = 0.03;
const i32 LEARNING_STEPS_AMOUNT = 10000;
const i32 MINIMAL_FIGURE_RADIUS = 6;

enum FigureTypes {
    H,
    O,
    P,
    C
};

struct Coordinate {
    i32 row;
    i32 column;
};

std::array<std::array<i32, LAYER_DIMENSION>, LAYER_DIMENSION> R{};
std::array<std::array<i32, LAYER_DIMENSION>, LAYER_DIMENSION> A{};
std::array<std::array<std::array<Coordinate, LAYER_DIMENSION>, LAYER_DIMENSION>, LAYER_DIMENSION> S{};
std::array<std::array<std::array<double, 4>, LAYER_DIMENSION>, LAYER_DIMENSION> W{};

void init() {
    for (i32 row = 0; row < LAYER_DIMENSION; ++row)
        for (i32 column = 0; column < LAYER_DIMENSION; ++column)
            for (i32 depth = 0; depth < LAYER_DIMENSION; ++depth) {
                S[row][column][depth].row = rand() % LAYER_DIMENSION;
                S[row][column][depth].column = rand() % LAYER_DIMENSION;
            }
}

void write_image(FigureTypes figure_type, bool draw_image = false) {
    std::memset(R.data(), 0, sizeof(R));

    i32 image_radius = std::clamp(rand() % LAYER_DIMENSION_THIRD,
                                  MINIMAL_FIGURE_RADIUS,
                                  LAYER_DIMENSION_THIRD);
    auto image_diameter = image_radius << 1;

    auto center = Coordinate{
            image_radius + (rand() % (LAYER_DIMENSION - image_diameter)),
            image_radius + (rand() % (LAYER_DIMENSION - image_diameter))
    };

    switch (figure_type) {
        case H:
            for (i32 i = -image_radius; i <= image_radius; ++i) {
                i32 row = center.row + i;
                i32 column = center.column - image_radius;
                R[row][column] = 1;
                column = center.column + image_radius;
                R[row][column] = 1;
                row = center.row;
                column = center.column + i;
                R[row][column] = 1;
            }
            break;
        case O:
            for (i32 i = 1; i < 100; ++i) {
                i32 row = center.row + static_cast<i32>(image_radius * cos(i));
                i32 column = center.column + static_cast<i32>(image_radius * sin(i));
                R[row][column] = 1;
            }
            break;
        case P: {
            auto start_point = Coordinate{center.row - image_radius, center.column - image_radius};

            for (i32 i = 0; i <= image_diameter; ++i) {
                R[0 + start_point.row][i + start_point.column] = 1;
                for (i32 u = 1; u < image_diameter; ++u)
                    if (i == 0 || i == image_diameter)
                        R[u + start_point.row][i + start_point.column] = 1;
            }
        }
            break;
        case C:
            auto start_point = Coordinate{center.row - image_radius, center.column - image_radius};

            for (i32 i = 0; i <= image_diameter; ++i) {
                if (i != 0 && i != image_diameter)
                    R[i + start_point.row][0 + start_point.column] = 1;
                for (i32 u = 1; u < image_diameter; ++u)
                    if (i == 0 || i == image_diameter)
                        R[i + start_point.row][u + start_point.column] = 1;
            }
    }

    if (draw_image) {
        switch (figure_type) {
            case H:
                std::cout << "Draw a H" << std::endl;
                break;
            case O:
                std::cout << "Draw a O" << std::endl;
                break;
            case P:
                std::cout << "Draw a P" << std::endl;
                break;
            case C:
                std::cout << "Draw a C" << std::endl;
        }

        for (i32 row = 0; row < LAYER_DIMENSION; ++row) {
            for (i32 column = 0; column < LAYER_DIMENSION; ++column)
                if (R[row][column] == 0)
                    std::cout << "_";
                else
                    std::cout << "*";

            std::cout << std::endl;
        }
    }
}

void mapping() {
    std::memset(A.data(), 0, sizeof(A));

    for (i32 row = 0; row < LAYER_DIMENSION; ++row)
        for (i32 column = 0; column < LAYER_DIMENSION; ++column)
            if (R[row][column] == 1)
                for (i32 depth = 0; depth < LAYER_DIMENSION; ++depth) {
                    auto coordinate = S[row][column][depth];
                    auto r = coordinate.row;
                    auto c = coordinate.column;
                    ++A[r][c];
                }
    for (i32 row = 0; row < LAYER_DIMENSION; ++row)
        for (i32 column = 0; column < LAYER_DIMENSION; ++column)
            A[row][column] = A[row][column] > MINIMUM_RECOGNITION_THRESHOLD ? 1 : 0;
};

byte recognize(bool show_guess = false) {
    std::array<double, 4> e{};
    byte result = 0;

    for (i32 depth = 0; depth < 4; ++depth) {
        for (i32 row = 0; row < LAYER_DIMENSION; ++row)
            for (i32 column = 0; column < LAYER_DIMENSION; ++column)
                e[depth] += A[row][column] * W[row][column][depth];

        if (e[depth] > 0)
            result |= (byte) (1 << (3 - depth));
        else
            result &= ~(byte) (1 << (3 - depth));
    }

    if (show_guess) {
        switch (result) {
            case 8:
                std::cout << "I think is a H" << std::endl;
                break;
            case 4:
                std::cout << "I think is a O" << std::endl;
                break;
            case 2:
                std::cout << "I think is a P" << std::endl;
                break;
            case 1:
                std::cout << "I think is a C" << std::endl;
                break;
            default:
                std::cout << "Unrecognized type" << std::endl;
        }

        std::cin.get();
    }

    return result;
}

void teach(byte generated_figure_type, byte recognized_figure_type) {
    for (i32 depth = 0; depth < 4; ++depth)
        if ((generated_figure_type >> (3 - depth) & 1) != (recognized_figure_type >> (3 - depth) & 1))
            for (i32 row = 0; row < LAYER_DIMENSION; ++row)
                for (i32 column = 0; column < LAYER_DIMENSION; ++column)
                    if (A[row][column] == 1) {
                        if ((generated_figure_type >> (3 - depth) & 1) == 0)
                            W[row][column][depth] -= LEARNING_WEIGHT;
                        else
                            W[row][column][depth] += LEARNING_WEIGHT;
                    }
};

int main() {
    srand(time(nullptr));

    i32 right_answers_amount = 0;
    bool is_check_step = false;

    init();

    for (i32 step = 1; step <= LEARNING_STEPS_AMOUNT; ++step) {
        auto generated_figure_type = step % 4;

        write_image(static_cast<FigureTypes>(generated_figure_type), is_check_step);

        mapping();

        auto recognized_figure_type = recognize(is_check_step);

        if (byte(8) >> generated_figure_type == recognized_figure_type)
            right_answers_amount += 1;
        else
            teach(byte(8) >> generated_figure_type, recognized_figure_type);

        std::cout << "Step " << step
                  << " Right answers: " << static_cast<double>(right_answers_amount) / static_cast<double>(step) * 100
                  << "%" << std::endl;

        if (step == LEARNING_STEPS_AMOUNT - 20)
            is_check_step = true;
    }
}