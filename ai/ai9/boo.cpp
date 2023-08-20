#include <cstdint>
#include <array>
#include <random>
#include <iostream>

using ufast32 = uint_fast32_t;

const ufast32 INPUT_NEURONS_AMOUNT = 4;
const ufast32 HIDDEN_NEURONS_AMOUNT = 3;
const ufast32 OUTPUT_NEURONS_AMOUNT = 4;

const ufast32 STEPS_AMOUNT = 10000;
const ufast32 SAMPLES_AMOUNT = 18;
const double LEARNING_RATE = 0.2;

const std::array<std::string, OUTPUT_NEURONS_AMOUNT> ACTIONS_TITLES = {"Attack", "Run", "Wander", "Hide"};

struct NPC {
    ufast32 health_amount;
    bool has_knife;
    bool has_handgun;
    ufast32 enemies_amount;
    std::array<double, INPUT_NEURONS_AMOUNT> decision;
};

const NPC SAMPLES[SAMPLES_AMOUNT] = {
        {2, false, false, 0, {0.0, 0.0, 1.0, 0.0}},
        {2, false, false, 1, {0.0, 0.0, 1.0, 0.0}},
        {2, false, true,  1, {1.0, 0.0, 0.0, 0.0}},
        {2, false, true,  2, {1.0, 0.0, 0.0, 0.0}},
        {2, true,  false, 2, {0.0, 0.0, 0.0, 1.0}},
        {2, true,  false, 1, {1.0, 0.0, 0.0, 0.0}},

        {1, false, false, 0, {0.0, 0.0, 1.0, 0.0}},
        {1, false, false, 1, {0.0, 0.0, 0.0, 1.0}},
        {1, false, true,  1, {1.0, 0.0, 0.0, 0.0}},
        {1, false, true,  2, {0.0, 0.0, 0.0, 1.0}},
        {1, true,  false, 2, {0.0, 0.0, 0.0, 1.0}},
        {1, true,  false, 1, {0.0, 0.0, 0.0, 1.0}},

        {0, false, false, 0, {0.0, 0.0, 1.0, 0.0}},
        {0, false, false, 1, {0.0, 0.0, 0.0, 1.0}},
        {0, false, true,  1, {0.0, 0.0, 0.0, 1.0}},
        {0, false, true,  2, {0.0, 1.0, 0.0, 0.0}},
        {0, true,  false, 2, {0.0, 1.0, 0.0, 0.0}},
        {0, true,  false, 1, {0.0, 0.0, 0.0, 1.0}}
};

std::array<double, INPUT_NEURONS_AMOUNT> input_layer;
std::array<double, HIDDEN_NEURONS_AMOUNT> hidden_layer;
std::array<double, OUTPUT_NEURONS_AMOUNT> output_layer;
std::array<double, OUTPUT_NEURONS_AMOUNT> expected_answers;

std::array<std::array<double, INPUT_NEURONS_AMOUNT + 1>, HIDDEN_NEURONS_AMOUNT> input_hidden_weights;
std::array<std::array<double, HIDDEN_NEURONS_AMOUNT + 1>, OUTPUT_NEURONS_AMOUNT> hidden_output_weights;

inline std::mt19937 &generator() {
    static thread_local std::mt19937 gen(std::random_device{}());
    return gen;
}

namespace neuro {
    ufast32 rand(ufast32 min, ufast32 max) {
        std::uniform_int_distribution<ufast32> dist(min, max);
        return dist(generator());
    }

    double rand(double min, double max) {
        std::uniform_real_distribution<double> dist(min, max);
        return dist(generator());
    }
}

void assign_weights() {
    for (ufast32 i = 0; i < INPUT_NEURONS_AMOUNT + 1; ++i)
        for (ufast32 j = 0; j < HIDDEN_NEURONS_AMOUNT; ++j)
            input_hidden_weights[i][j] = neuro::rand(-0.5, 0.5);

    for (ufast32 i = 0; i < HIDDEN_NEURONS_AMOUNT + 1; ++i)
        for (ufast32 j = 0; j < OUTPUT_NEURONS_AMOUNT; ++j)
            hidden_output_weights[i][j] = neuro::rand(-0.5, 0.5);
}

inline double sigmoid(double value) {
    return 1.0 / (1.0 + exp(-value));
}

inline double sigmoid_derivative(double value) {
    return value * (1.0 - value);
}

void feed_forward() {
    for (ufast32 i = 0; i < HIDDEN_NEURONS_AMOUNT; ++i) {
        auto sum = 0.0;

        for (ufast32 j = 0; j < INPUT_NEURONS_AMOUNT; ++j)
            sum += input_layer[j] * input_hidden_weights[j][i];

        sum += input_hidden_weights[INPUT_NEURONS_AMOUNT][i];
        hidden_layer[i] = sigmoid(sum);
    }

    for (ufast32 i = 0; i < OUTPUT_NEURONS_AMOUNT; ++i) {
        auto sum = 0.0;

        for (ufast32 j = 0; j < HIDDEN_NEURONS_AMOUNT; ++j)
            sum += hidden_layer[j] * (hidden_output_weights[j][i]);

        sum += hidden_output_weights[HIDDEN_NEURONS_AMOUNT][i];
        output_layer[i] = sigmoid(sum);
    }
}

void back_propagation() {
    std::array<double, HIDDEN_NEURONS_AMOUNT> hidden_layer_errors_rate{};
    std::array<double, OUTPUT_NEURONS_AMOUNT> output_layer_errors_rate{};

    for (ufast32 i = 0; i < OUTPUT_NEURONS_AMOUNT; ++i)
        output_layer_errors_rate[i] = (expected_answers[i] - output_layer[i]) * sigmoid_derivative(output_layer[i]);

    for (ufast32 i = 0; i < HIDDEN_NEURONS_AMOUNT; ++i) {
        hidden_layer_errors_rate[i] = 0.0;

        for (ufast32 j = 0; j < OUTPUT_NEURONS_AMOUNT; ++j)
            hidden_layer_errors_rate[i] += output_layer_errors_rate[j] * hidden_output_weights[i][j];

        hidden_layer_errors_rate[i] *= sigmoid_derivative(hidden_layer[i]);
    }

    for (ufast32 i = 0; i < OUTPUT_NEURONS_AMOUNT; ++i) {
        for (ufast32 j = 0; j < HIDDEN_NEURONS_AMOUNT; ++j)
            hidden_output_weights[j][i] += (LEARNING_RATE * output_layer_errors_rate[i] * hidden_layer[j]);

        hidden_output_weights[HIDDEN_NEURONS_AMOUNT][i] += (LEARNING_RATE * output_layer_errors_rate[i]);
    }

    for (ufast32 i = 0; i < HIDDEN_NEURONS_AMOUNT; ++i) {
        for (ufast32 j = 0; j < INPUT_NEURONS_AMOUNT; ++j)
            input_hidden_weights[j][i] += (LEARNING_RATE * hidden_layer_errors_rate[i] * input_layer[j]);

        input_hidden_weights[INPUT_NEURONS_AMOUNT][i] += (LEARNING_RATE * hidden_layer_errors_rate[i]);
    }
}


void training() {
    ufast32 sample_index = 0;

    for (ufast32 step = 0; step < STEPS_AMOUNT; step++) {
        sample_index = (sample_index + 1) % SAMPLES_AMOUNT;

        input_layer[0] = SAMPLES[sample_index].health_amount;
        input_layer[1] = SAMPLES[sample_index].has_knife;
        input_layer[2] = SAMPLES[sample_index].has_handgun;
        input_layer[3] = SAMPLES[sample_index].enemies_amount;

        expected_answers[0] = SAMPLES[sample_index].decision[0];
        expected_answers[1] = SAMPLES[sample_index].decision[1];
        expected_answers[2] = SAMPLES[sample_index].decision[2];
        expected_answers[3] = SAMPLES[sample_index].decision[3];

        feed_forward();

        auto error_rate = 0.0;
        for (ufast32 i = 0; i < OUTPUT_NEURONS_AMOUNT; ++i)
            error_rate += pow(SAMPLES[sample_index].decision[i] - output_layer[i], 2);

        error_rate *= 0.5;

        std::cout << "Step = " << step << " mse = " << error_rate << std::endl;

        back_propagation();
    }
}

ufast32 max_index(const std::array<double, 4> &sequence) {
    auto max_iter = std::max_element(sequence.begin(), sequence.end());
    int sel = std::distance(sequence.begin(), max_iter);
    return sel;
}

void check() {
    ufast32 right_answers_amount = 0;
    for (const auto &i : SAMPLES) {
        input_layer[0] = i.health_amount;
        input_layer[1] = i.has_knife;
        input_layer[2] = i.has_handgun;
        input_layer[3] = i.enemies_amount;

        expected_answers[0] = i.decision[0];
        expected_answers[1] = i.decision[1];
        expected_answers[2] = i.decision[2];
        expected_answers[3] = i.decision[3];

        feed_forward();

        auto output_result = max_index(output_layer);
        auto expected_result = max_index(expected_answers);
        if (output_result != expected_result) {
            std::cout << input_layer[0]
                      << input_layer[1]
                      << input_layer[2]
                      << input_layer[3]
                      << " "
                      << ACTIONS_TITLES[output_result]
                      << " -> "
                      << ACTIONS_TITLES[expected_result] << std::endl;
        } else
            ++right_answers_amount;
    }

    std::cout << "Network is " << (float) right_answers_amount / SAMPLES_AMOUNT * 100.0 << " correct" << std::endl;
}

void test() {
    input_layer = {2.0, 1.0, 1.0, 1.0};
    feed_forward();
    std::cout << "2111 Action " << ACTIONS_TITLES[max_index(output_layer)] << std::endl;

    input_layer = {1.0, 1.0, 1.0, 2.0};
    feed_forward();
    std::cout << "1112 Action " << ACTIONS_TITLES[max_index(output_layer)] << std::endl;

    input_layer = {0.0, 0.0, 0.0, 0.0};
    feed_forward();
    std::cout << "0000 Action " << ACTIONS_TITLES[max_index(output_layer)] << std::endl;

    input_layer = {0.0, 1.0, 1.0, 1.0};
    feed_forward();
    std::cout << "0111 Action " << ACTIONS_TITLES[max_index(output_layer)] << std::endl;

    input_layer = {2.0, 0.0, 1.0, 3.0};
    feed_forward();
    std::cout << "2013 Action " << ACTIONS_TITLES[max_index(output_layer)] << std::endl;

    input_layer = {2.0, 1.0, 0.0, 3.0};
    feed_forward();
    std::cout << "2103 Action " << ACTIONS_TITLES[max_index(output_layer)] << std::endl;

    input_layer = {0.0, 1.0, 0.0, 3.0};
    feed_forward();
    std::cout << "0103 Action " << ACTIONS_TITLES[max_index(output_layer)] << std::endl;
}

int main() {
    assign_weights();

    training();
    check();
    test();
}
