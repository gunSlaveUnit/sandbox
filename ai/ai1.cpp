#include <iostream>
#include <array>
#include <cstring>
#include <cmath>

using i32 = int_fast32_t;

const i32 LAYER_DIMENSION = 20;
const i32 LAYER_DIMENSION_HALF = LAYER_DIMENSION >> 1;
const i32 MINIMUM_RECOGNITION_THRESHOLD = 3;
const double LEARNING_WEIGHT = 0.03;
const i32 LEARNING_STEPS_AMOUNT = 10000;
const i32 MINIMAL_FIGURE_RADIUS = 3;

enum FigureTypes {
    ZERO, CROSS
};

struct Coordinate {
    i32 row;
    i32 column;
};

std::array<std::array<i32, LAYER_DIMENSION>, LAYER_DIMENSION> R{};
std::array<std::array<i32, LAYER_DIMENSION>, LAYER_DIMENSION> A{};
std::array<std::array<std::array<Coordinate, LAYER_DIMENSION>, LAYER_DIMENSION>, LAYER_DIMENSION> S{};
std::array<std::array<double, LAYER_DIMENSION>, LAYER_DIMENSION> W{};

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

    i32 image_radius = std::clamp(rand() % LAYER_DIMENSION_HALF,
                                  MINIMAL_FIGURE_RADIUS,
                                  LAYER_DIMENSION_HALF);
    auto image_diameter = image_radius << 1;

    auto center = Coordinate{
            image_radius + (rand() % (LAYER_DIMENSION - image_diameter)),
            image_radius + (rand() % (LAYER_DIMENSION - image_diameter))
    };

    switch (figure_type) {
        case ZERO:
            for (i32 i = 1; i < 150; ++i) {
                i32 row = center.row + static_cast<i32>(image_radius * cos(i));
                i32 column = center.column + static_cast<i32>(image_radius * sin(i));
                R[row][column] = 1;
            }
            break;
        case CROSS:
            for (i32 i = -image_radius; i <= image_radius; ++i) {
                i32 row = center.row + i;
                i32 column = center.row + i;
                R[row][column] = 1;
                column = center.column - i;
                R[row][column] = 1;
            }
    }

    if (draw_image) {
        switch (figure_type) {
            case ZERO:
                std::cout << "Draw a zero" << std::endl;
                break;
            case CROSS:
                std::cout << "Draw a cross" << std::endl;
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

FigureTypes recognize(bool show_guess = false) {
    auto e = 0.0;
    for (i32 row = 0; row < LAYER_DIMENSION; ++row)
        for (i32 column = 0; column < LAYER_DIMENSION; ++column)
            e += A[row][column] * W[row][column];

    if (show_guess) {
        std::cout << std::endl;

        if (e > 0)
            std::cout << "I think is a cross" << std::endl;
        else
            std::cout << "I think is a zero" << std::endl;

        std::cin.get();
    }

    return static_cast<FigureTypes>(e > 0);
};

void teach(FigureTypes figure_type) {
    for (i32 row = 0; row < LAYER_DIMENSION; ++row)
        for (i32 column = 0; column < LAYER_DIMENSION; ++column)
            if (A[row][column] == 1)
                switch (figure_type) {
                    case ZERO:
                        W[row][column] -= LEARNING_WEIGHT;
                        break;
                    case CROSS:
                        W[row][column] += LEARNING_WEIGHT;
                }
};

int main() {
    srand(time(nullptr));

    i32 right_answers_amount = 0;
    bool is_check_step = false;

    init();

    for (i32 step = 1; step <= LEARNING_STEPS_AMOUNT; ++step) {
        auto generated_figure_type = step % 2;

        write_image(static_cast<FigureTypes>(generated_figure_type), is_check_step);

        mapping();

        auto recognized_figure_type = recognize(is_check_step);

        if (generated_figure_type == recognized_figure_type)
            right_answers_amount += 1;
        else
            teach(static_cast<FigureTypes>(generated_figure_type));

        std::cout << "Step " << step
                  << " Right answers: " << static_cast<double>(right_answers_amount) / static_cast<double>(step) * 100
                  << "%" << std::endl;
        if (step == LEARNING_STEPS_AMOUNT - 20)
            is_check_step = true;
    }
}