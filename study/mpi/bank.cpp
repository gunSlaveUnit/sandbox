//
// Dummy bank
//

#include <array>
#include <random>
#include <cstring>
#include <iostream>

#include <mpi.h>

enum RequestStatus {
    success,
    wait,
    bankrupt
};

enum RequestPart {
    operation_type,
    amount
};

enum RequestType {
    refill,
    widthdrawal
};

constexpr int REQUEST_LENGTH = 2;
constexpr int POSSIBLE_OPERATIONS_AMOUNT = 2;

std::array<int, REQUEST_LENGTH> generate_request() {
    std::random_device os_seed;
    const auto seed = os_seed();

    std::mt19937 generator(seed);
    constexpr int MIN_AMOUNT = 100;
    constexpr int MAX_AMOUNT = 300;
    std::uniform_int_distribution<int> distribute(MIN_AMOUNT, MAX_AMOUNT);

    auto operation = distribute(generator) % POSSIBLE_OPERATIONS_AMOUNT;
    auto amount = distribute(generator);

    return std::array<int, REQUEST_LENGTH>{operation, amount};
}

int main() {
    int rank{}, size{};
    if (MPI_Init(nullptr, nullptr) != MPI_SUCCESS
        || MPI_Comm_rank(MPI_COMM_WORLD, &rank) != MPI_SUCCESS
        || MPI_Comm_size(MPI_COMM_WORLD, &size) != MPI_SUCCESS) {
        std::cerr << "ERROR: unable to initialize MPI\n";
        MPI_Finalize();
    }

    constexpr int BANK = 0;
    auto is_client = rank != BANK;
    auto bank_response_status = RequestStatus::success;
    std::array<int, REQUEST_LENGTH> request{};
    const std::string OPERATION_TITLES[POSSIBLE_OPERATIONS_AMOUNT]{
            "REFILL",
            "WIDTHDRAWAL"
    };
    if (is_client) {
        while (bank_response_status != RequestStatus::bankrupt) {
            if (bank_response_status != RequestStatus::wait) {
                request = generate_request();

                MPI_Send(
                        request.data(),
                        request.size(),
                        MPI_INT,
                        BANK,
                        0,
                        MPI_COMM_WORLD
                );
            }

            MPI_Recv(
                    &bank_response_status,
                    1,
                    MPI_INT,
                    BANK,
                    0,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE
            );
        }

        std::cout
                << "CLIENT "
                << rank
                << ": exit"
                << std::endl;
    } else {
        constexpr bool IS_FCFS_ENABLED = true;
        constexpr int MAX_REQUESTS_PER_DAY = 20;
        std::array<int, MAX_REQUESTS_PER_DAY * REQUEST_LENGTH> queue{};
        // Change only with REQUEST_LENGTH
        auto current_queue_position = queue.data();
        auto bank_balance = 1000;

        while (bank_response_status != RequestStatus::bankrupt) {
            auto messages_amount = size - (current_queue_position - queue.data()) / REQUEST_LENGTH - 1;
            for (int i = 0; i < messages_amount; ++i) {
                MPI_Status request_status;
                MPI_Recv(
                        request.data(),
                        request.size(),
                        MPI_INT,
                        MPI_ANY_SOURCE,
                        0,
                        MPI_COMM_WORLD,
                        &request_status
                );

                std::cout
                        << "BANK: "
                        << OPERATION_TITLES[request[RequestPart::operation_type]]
                        << " "
                        << request[RequestPart::amount]
                        << " from client "
                        << request_status.MPI_SOURCE
                        << std::endl;

                if (request[RequestPart::operation_type] == RequestType::refill) {
                    bank_balance += request[RequestPart::amount];

                    bank_response_status = RequestStatus::success;

                    MPI_Send(
                            &bank_response_status,
                            1,
                            MPI_INT,
                            request_status.MPI_SOURCE,
                            0,
                            MPI_COMM_WORLD
                    );

                    std::cout << "BANK: " << bank_balance << std::endl;
                } else {
                    if (request[RequestPart::amount] <= bank_balance) {
                        bank_balance -= request[RequestPart::amount];

                        bank_response_status = RequestStatus::success;

                        MPI_Send(
                                &bank_response_status,
                                1,
                                MPI_INT,
                                request_status.MPI_SOURCE,
                                0,
                                MPI_COMM_WORLD
                        );

                        std::cout << "BANK: " << bank_balance << std::endl;
                    } else {
                        bank_response_status = RequestStatus::wait;

                        MPI_Send(
                                &bank_response_status,
                                1,
                                MPI_INT,
                                request_status.MPI_SOURCE,
                                0,
                                MPI_COMM_WORLD
                        );

                        // Don't create a new buffer, need a client rank for queue processing later
                        request[RequestPart::operation_type] = request_status.MPI_SOURCE;

                        memcpy(
                                current_queue_position,
                                request.data(),
                                sizeof(int) * REQUEST_LENGTH
                        );

                        current_queue_position += REQUEST_LENGTH;

                        std::cout
                                << "BANK: Can't handle a request. Pushed to queue"
                                << std::endl;
                    }
                }
            }

            // Change only with REQUEST_LENGTH
            auto processed_client_position = queue.data();

            while (processed_client_position != current_queue_position) {
                auto amount = processed_client_position[RequestPart::amount];

                if (amount > bank_balance) {
                    if (IS_FCFS_ENABLED) {
                        std::cout
                                << "FCFS: can't send "
                                << amount <<
                                " to client "
                                << processed_client_position[RequestPart::operation_type]
                                << std::endl;

                        std::cout << "Queue" << std::endl;
                        for (auto client = processed_client_position;
                             client < current_queue_position; client += REQUEST_LENGTH)
                            std::cout
                                    << "Client: "
                                    << client[RequestPart::operation_type]
                                    << " Amount: "
                                    << client[RequestPart::amount]
                                    << std::endl;

                        break;
                    }

                    processed_client_position += REQUEST_LENGTH;

                    continue;
                }

                bank_balance -= amount;

                bank_response_status = RequestStatus::success;

                auto source = queue[RequestPart::operation_type];

                MPI_Send(
                        &bank_response_status,
                        1,
                        MPI_INT,
                        source,
                        0,
                        MPI_COMM_WORLD
                );

                std::cout << "BANK: send " << amount << " to CLIENT " << source << std::endl;

                std::cout << "BANK: " << bank_balance << std::endl;

                for (auto client = processed_client_position;
                     client < current_queue_position; client += REQUEST_LENGTH) {
                    auto next_client = client + REQUEST_LENGTH;

                    memcpy(client, next_client, sizeof(int) * REQUEST_LENGTH);
                }

                current_queue_position -= REQUEST_LENGTH;
            }

            auto is_bank_bankrupt = (current_queue_position - queue.data()) / REQUEST_LENGTH == size - 1;
            if (is_bank_bankrupt) {
                bank_response_status = RequestStatus::bankrupt;

                for (auto i = 0; i < size; ++i)
                    if (i != BANK)
                        MPI_Send(
                                &bank_response_status,
                                1,
                                MPI_INT,
                                i,
                                0,
                                MPI_COMM_WORLD
                        );

                std::cout << "BANK: bankrupt" << std::endl;
                std::cout << "BANK: " << bank_balance << std::endl;
            }
        }

        std::cout << "BANK: exit " << std::endl;
    }

    MPI_Finalize();
}
