#include <iostream>
#include <functional>
#include <limits>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <vector>

#include "joining_thread_group.h"

namespace {
constexpr int iterations = 10'000;

struct Account {
    explicit Account(int initial_balance) : balance(initial_balance) {
        if (initial_balance < 0) {
            throw std::invalid_argument("account balance must be non-negative");
        }
    }

    int balance;
    std::mutex mutex;
};

bool transfer(Account& from, Account& to, int amount) {
    if (&from == &to || amount <= 0) {
        return false;
    }
    const std::scoped_lock lock(from.mutex, to.mutex);
    const int maximum = std::numeric_limits<int>::max();
    if (from.balance < amount || to.balance > maximum - amount) {
        return false;
    }
    from.balance -= amount;
    to.balance += amount;
    return true;
}
}  // namespace

int main() {
    std::mutex counterMutex;
    int counter = 0;
    auto safeIncrement = [&counterMutex, &counter] {
        for (int i = 0; i < iterations; ++i) {
            const std::lock_guard<std::mutex> lock(counterMutex);
            ++counter;
        }
    };

    week5::JoiningThreadGroup workers;
    workers.start(safeIncrement);
    workers.start(safeIncrement);
    workers.join_all();

    Account first{100};
    Account second{100};
    const bool rejects_same_account = !transfer(first, first, 10);
    const bool rejects_negative_amount = !transfer(first, second, -1);

    bool rejects_negative_initial_balance = false;
    try {
        const Account invalid{-1};
        static_cast<void>(invalid);
    } catch (const std::invalid_argument&) {
        rejects_negative_initial_balance = true;
    }

    Account overflow_source{1};
    Account full_destination{std::numeric_limits<int>::max()};
    const bool rejects_destination_overflow =
        !transfer(overflow_source, full_destination, 1);
    const bool overflow_failure_preserves_balances =
        overflow_source.balance == 1 &&
        full_destination.balance == std::numeric_limits<int>::max();

    Account boundary_source{1};
    Account boundary_destination{std::numeric_limits<int>::max() - 1};
    const bool accepts_exact_upper_boundary =
        transfer(boundary_source, boundary_destination, 1) &&
        boundary_source.balance == 0 &&
        boundary_destination.balance == std::numeric_limits<int>::max();

    week5::JoiningThreadGroup transfers;
    transfers.start(transfer, std::ref(first), std::ref(second), 10);
    transfers.start(transfer, std::ref(second), std::ref(first), 20);
    transfers.join_all();

    const bool passed = counter == 2 * iterations &&
                        rejects_same_account && rejects_negative_amount &&
                        rejects_negative_initial_balance &&
                        rejects_destination_overflow &&
                        overflow_failure_preserves_balances &&
                        accepts_exact_upper_boundary &&
                        first.balance + second.balance == 200 &&
                        first.balance == 110 && second.balance == 90;
    std::cout << "counter=" << counter << ", balances=" << first.balance << '+'
              << second.balance << '\n';
    std::cout << (passed ? "mutex checks passed\n" : "mutex checks failed\n");
    return passed ? 0 : 1;
}
