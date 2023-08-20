#include <string>
#include <array>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <random>
#include <cstring>

using ufast32 = uint_fast32_t;

const ufast32 SENSORS_AMOUNT = 34;
const ufast32 TRAINING_SAMPLES_AMOUNT = 300;
const ufast32 CONTROL_SAMPLES_AMOUNT = 151;
const ufast32 STEPS_AMOUNT = 10000;
const double LEARNING_RATE = 0.2;
const ufast32 HIDDEN_NEURONS_AMOUNT = SENSORS_AMOUNT + 1;
const ufast32 OUTPUT_NEURONS_AMOUNT = 1;

struct Sensor {
    std::vector<double> signals;
    bool result;
};

std::vector<Sensor> training_samples{};
std::vector<Sensor> control_samples{};

std::array<double, SENSORS_AMOUNT> input_layer;
std::array<double, HIDDEN_NEURONS_AMOUNT> hidden_layer;
std::array<double, OUTPUT_NEURONS_AMOUNT> output_layer;
std::array<std::array<double, SENSORS_AMOUNT + 1>, HIDDEN_NEURONS_AMOUNT> input_hidden_weights;
std::array<std::array<double, HIDDEN_NEURONS_AMOUNT + 1>, OUTPUT_NEURONS_AMOUNT> hidden_output_weights;
bool expected_answer;

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

Sensor parse_sample(const std::string &line) {
    std::vector<double> signal_data;
    signal_data.reserve(SENSORS_AMOUNT);

    std::stringstream ss(line);

    for (double i; ss >> i;) {
        signal_data.push_back(i);
        if (ss.peek() == ',')
            ss.ignore();
    }

    char result_char = line[line.length() - 2];
    bool result{result_char == 'g'};

    return Sensor{signal_data, result};
}

void load_sample(const std::string &filename, std::vector<Sensor> &destination) {
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
    for (ufast32 i = 0; i < SENSORS_AMOUNT + 1; ++i)
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

        for (ufast32 j = 0; j < SENSORS_AMOUNT; ++j)
            sum += input_layer[j] * input_hidden_weights[j][i];

        sum += input_hidden_weights[SENSORS_AMOUNT][i];
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

    double output_layer_error_rate = (expected_answer - output_layer[0]) * sigmoid_derivative(output_layer[0]);

    for (ufast32 i = 0; i < OUTPUT_NEURONS_AMOUNT; ++i) {
        for (ufast32 j = 0; j < HIDDEN_NEURONS_AMOUNT; ++j)
            hidden_output_weights[j][i] += (LEARNING_RATE * output_layer_error_rate * hidden_layer[j]);

        hidden_output_weights[HIDDEN_NEURONS_AMOUNT][i] += (LEARNING_RATE * output_layer_error_rate);
    }

    for (ufast32 i = 0; i < HIDDEN_NEURONS_AMOUNT; ++i) {
        hidden_layer_errors_rate[i] = 0.0;

        for (ufast32 j = 0; j < OUTPUT_NEURONS_AMOUNT; ++j)
            hidden_layer_errors_rate[i] += output_layer_error_rate * hidden_output_weights[i][j];

        hidden_layer_errors_rate[i] *= sigmoid_derivative(hidden_layer[i]);
    }

    for (ufast32 i = 0; i < HIDDEN_NEURONS_AMOUNT; ++i) {
        for (ufast32 j = 0; j < SENSORS_AMOUNT; ++j)
            input_hidden_weights[j][i] += (LEARNING_RATE * hidden_layer_errors_rate[i] * input_layer[j]);

        input_hidden_weights[SENSORS_AMOUNT][i] += (LEARNING_RATE * hidden_layer_errors_rate[i]);
    }
}

void training() {
    ufast32 sample_index = 0;

    for (ufast32 step = 0; step < STEPS_AMOUNT; step++) {
        sample_index = (sample_index + 1) % TRAINING_SAMPLES_AMOUNT;

        std::memcpy(input_layer.data(),
                    training_samples[sample_index].signals.data(),
                    sizeof(double) * SENSORS_AMOUNT);

        expected_answer = training_samples[sample_index].result;

        feed_forward();

        auto error_rate = pow(expected_answer - output_layer[0], 2);
        std::cout << "[Step = " << step << "] [Error = " << error_rate << "]" << std::endl;

        back_propagation();
    }
}

void check() {
    ufast32 right_answers_amount = 0;
    for (ufast32 sample_index = 0; sample_index < CONTROL_SAMPLES_AMOUNT; ++sample_index) {
        std::memcpy(input_layer.data(),
                    training_samples[sample_index].signals.data(),
                    sizeof(double) * SENSORS_AMOUNT);

        expected_answer = training_samples[sample_index].result;

        feed_forward();

        auto current = std::round(output_layer[0]);
        std::cout << "[Expected:" << expected_answer << "][Network answer:" << current << "]" << std::endl;

        if (current == expected_answer)
            ++right_answers_amount;
    }

    std::cout << "Network is " << (float) right_answers_amount / CONTROL_SAMPLES_AMOUNT * 100.0 << " correct"
              << std::endl;
}

int main() {
    load_training_sample("../training.csv");
    load_control_sample("../control.csv");
    std::cout << "Samples are ready" << std::endl;

    assign_weights();
    std::cout << "Network initialized" << std::endl;

    std::cout << "Start training" << std::endl;
    training();

    std::cout << "Start checking" << std::endl;
    check();
}