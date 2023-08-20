#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <random>
#include <cstring>

using ufast32 = uint_fast32_t;

const ufast32 MILK_PARAMS_AMOUNT = 7;
const ufast32 TRAINING_SAMPLES_AMOUNT = 1000;
const ufast32 CONTROL_SAMPLES_AMOUNT = 59;
const ufast32 STEPS_AMOUNT = 15000;
const double LEARNING_RATE = 0.2;
const ufast32 HIDDEN_NEURONS_AMOUNT = MILK_PARAMS_AMOUNT + 1;
const ufast32 OUTPUT_NEURONS_AMOUNT = 3;

struct MilkSort {
    std::vector<double> params;
    std::array<double, OUTPUT_NEURONS_AMOUNT> decision;
};

std::vector<MilkSort> training_samples{};
std::vector<MilkSort> control_samples{};

std::array<double, MILK_PARAMS_AMOUNT> input_layer;
std::array<double, HIDDEN_NEURONS_AMOUNT> hidden_layer;
std::array<double, OUTPUT_NEURONS_AMOUNT> output_layer;
std::array<std::array<double, MILK_PARAMS_AMOUNT + 1>, HIDDEN_NEURONS_AMOUNT> input_hidden_weights;
std::array<std::array<double, HIDDEN_NEURONS_AMOUNT + 1>, OUTPUT_NEURONS_AMOUNT> hidden_output_weights;
std::array<double, OUTPUT_NEURONS_AMOUNT> expected_answers;

namespace neuro {
    inline std::mt19937 &generator() {
        static thread_local std::mt19937 gen(std::random_device{}());
        return gen;
    }

    ufast32 rand(ufast32 min, ufast32 max) {
        std::uniform_int_distribution<ufast32> dist(min, max);
        return dist(generator());
    }

    double rand(double min, double max) {
        std::uniform_real_distribution<double> dist(min, max);
        return dist(generator());
    }
}

MilkSort parse_sample(const std::string &line) {
    std::vector<double> signal_data;
    signal_data.reserve(MILK_PARAMS_AMOUNT);

    std::stringstream ss(line);

    for (ufast32 i = 0; i < MILK_PARAMS_AMOUNT; ++i) {
        double param;
        ss >> param;

        signal_data.push_back(param);

        if (ss.peek() == ',')
            ss.ignore();
    }

    int result;
    ss >> result;

    std::array<double, OUTPUT_NEURONS_AMOUNT> decision{};
    decision[result] = 1;

    return MilkSort{signal_data, decision};
}

void load_sample(const std::string &filename, std::vector<MilkSort> &destination) {
    std::ifstream dataset(filename);
    if (!dataset.is_open())
        throw std::runtime_error("ERROR: Failed to open dataset");

    std::string line;
    while (std::getline(dataset, line))
        destination.emplace_back(parse_sample(line));
}

void load_training_sample(const std::string &filename) {
    load_sample(filename, training_samples);
}

void load_control_sample(const std::string &filename) {
    load_sample(filename, control_samples);
}

void assign_weights() {
    for (ufast32 i = 0; i < MILK_PARAMS_AMOUNT + 1; ++i)
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

        for (ufast32 j = 0; j < MILK_PARAMS_AMOUNT; ++j)
            sum += input_layer[j] * input_hidden_weights[j][i];

        sum += input_hidden_weights[MILK_PARAMS_AMOUNT][i];
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

    for (ufast32 i = 0; i < OUTPUT_NEURONS_AMOUNT; ++i) {
        for (ufast32 j = 0; j < HIDDEN_NEURONS_AMOUNT; ++j)
            hidden_output_weights[j][i] += (LEARNING_RATE * output_layer_errors_rate[i] * hidden_layer[j]);

        hidden_output_weights[HIDDEN_NEURONS_AMOUNT][i] += (LEARNING_RATE * output_layer_errors_rate[i]);
    }

    for (ufast32 i = 0; i < HIDDEN_NEURONS_AMOUNT; ++i) {
        hidden_layer_errors_rate[i] = 0.0;

        for (ufast32 j = 0; j < OUTPUT_NEURONS_AMOUNT; ++j)
            hidden_layer_errors_rate[i] += output_layer_errors_rate[j] * hidden_output_weights[i][j];

        hidden_layer_errors_rate[i] *= sigmoid_derivative(hidden_layer[i]);
    }

    for (ufast32 i = 0; i < HIDDEN_NEURONS_AMOUNT; ++i) {
        for (ufast32 j = 0; j < MILK_PARAMS_AMOUNT; ++j)
            input_hidden_weights[j][i] += (LEARNING_RATE * hidden_layer_errors_rate[i] * input_layer[j]);

        input_hidden_weights[MILK_PARAMS_AMOUNT][i] += (LEARNING_RATE * hidden_layer_errors_rate[i]);
    }
}

void training() {
    ufast32 sample_index = 0;

    for (ufast32 step = 0; step < STEPS_AMOUNT; step++) {
        sample_index = (sample_index + 1) % TRAINING_SAMPLES_AMOUNT;

        std::memcpy(input_layer.data(),
                    training_samples[sample_index].params.data(),
                    sizeof(double) * MILK_PARAMS_AMOUNT);

        std::memcpy(expected_answers.data(),
                    training_samples[sample_index].decision.data(),
                    sizeof(double) * OUTPUT_NEURONS_AMOUNT);

        feed_forward();

        auto error_rate = 0.0;
        for (ufast32 i = 0; i < OUTPUT_NEURONS_AMOUNT; ++i)
            error_rate += pow(training_samples[sample_index].decision[i] - output_layer[i], 2);

        error_rate *= 0.5;

        std::cout << output_layer[0]  << " " << output_layer[1]<< " "<< output_layer[2]<< std::endl;

        back_propagation();
    }
}

ufast32 max_index(const std::array<double, 3> &sequence) {
    auto max_iter = std::max_element(sequence.begin(), sequence.end());
    int sel = std::distance(sequence.begin(), max_iter);
    return sel;
}

void check() {
    ufast32 right_answers_amount = 0;
    for (ufast32 sample_index = 0; sample_index < CONTROL_SAMPLES_AMOUNT; ++sample_index) {
        std::memcpy(input_layer.data(),
                    control_samples[sample_index].params.data(),
                    sizeof(double) * MILK_PARAMS_AMOUNT);

        std::memcpy(expected_answers.data(),
                    training_samples[sample_index].decision.data(),
                    sizeof(double) * OUTPUT_NEURONS_AMOUNT);

        feed_forward();

        auto output_result = max_index(output_layer);
        auto expected_result = max_index(expected_answers);

        if (output_result == expected_result)
            ++right_answers_amount;
    }

    std::cout << "Network is "
              << (float) right_answers_amount / CONTROL_SAMPLES_AMOUNT * 100.0
              << " correct" << std::endl;
}

int main() {
    load_training_sample("../train.csv");
    load_control_sample("../test.csv");
    std::cout << "Samples are ready" << std::endl;

    assign_weights();
    std::cout << "Network initialized" << std::endl;

    std::cout << "Start training" << std::endl;
    training();

    std::cout << "Start checking" << std::endl;
    check();
}
