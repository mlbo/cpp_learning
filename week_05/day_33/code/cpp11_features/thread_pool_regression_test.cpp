#include <future>
#include <iostream>
#include <stdexcept>

#include "thread_pool.h"

int main() {
    ThreadPool pool(2U);

    std::future<void> self_shutdown = pool.submit([&pool] { pool.shutdown(); });
    bool rejected_self_join = false;
    try {
        self_shutdown.get();
    } catch (const std::logic_error&) {
        rejected_self_join = true;
    }

    std::future<int> still_usable = pool.submit([] { return 42; });
    const bool task_completed = still_usable.get() == 42;
    pool.shutdown();

    bool rejects_after_external_shutdown = false;
    try {
        static_cast<void>(pool.submit([] {}));
    } catch (const std::runtime_error&) {
        rejects_after_external_shutdown = true;
    }

    if (!rejected_self_join || !task_completed || !rejects_after_external_shutdown) {
        std::cerr << "thread pool shutdown regression test failed\n";
        return 1;
    }

    std::cout << "worker shutdown is rejected; external shutdown drains and joins\n";
    return 0;
}
