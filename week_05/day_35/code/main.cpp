#include <array>
#include <cstddef>
#include <iostream>
#include <string_view>

struct ModuleContract {
    std::string_view name;
    std::size_t layer;
    std::string_view public_interface;
    std::string_view owner;
    std::string_view invariant;
    std::array<std::string_view, 3> dependencies;
};

struct ProjectContract {
    std::string_view stop_protocol;
    std::string_view verification;
};

template <std::size_t Count>
const ModuleContract* find_module(const std::array<ModuleContract, Count>& modules,
                                  std::string_view name) {
    for (const ModuleContract& module : modules) {
        if (module.name == name) {
            return &module;
        }
    }
    return nullptr;
}

int main() {
    constexpr std::array<ModuleContract, 5> modules{{
        {"tree_model", 0U, "Tree owns children with unique_ptr",
         "the root owner controls the complete tree lifetime",
         "every child has exactly one owning edge", {"", "", ""}},
        {"codec", 1U, "serialize(observer) and deserialize(string) -> unique_ptr<Tree>",
         "the caller owns the deserialized root",
         "accepted text has no empty, partial or trailing token",
         {"tree_model", "", ""}},
        {"algorithms", 1U, "max_path(observer) and reconstruct(ranges)",
         "algorithms borrow trees and return values or new roots",
         "inputs satisfy explicit range and overflow contracts",
         {"tree_model", "", ""}},
        {"executor", 1U, "submit(Task) -> future<Result>; shutdown()",
         "the queue owns pending tasks; workers own running calls",
         "each accepted task makes exactly one future ready", {"", "", ""}},
        {"tree_service", 2U, "decode -> validate -> schedule -> encode",
         "the service owns modules; request state is scoped to one operation",
         "dependency direction is service to interfaces, never back to the app",
         {"codec", "algorithms", "executor"}},
    }};
    constexpr ProjectContract project{
        "external owner rejects new tasks, drains accepted tasks, then joins workers",
        "unit, contract, sanitizer and concurrency tests cover values and failure paths",
    };

    bool valid = !project.stop_protocol.empty() && !project.verification.empty();
    bool has_service = false;
    bool has_executor = false;
    for (std::size_t index = 0; index < modules.size(); ++index) {
        const ModuleContract& module = modules[index];
        valid = valid && !module.name.empty() && !module.public_interface.empty() &&
                !module.owner.empty() && !module.invariant.empty();
        has_service = has_service || module.name == "tree_service";
        has_executor = has_executor || module.name == "executor";

        for (std::size_t other = index + 1U; other < modules.size(); ++other) {
            valid = valid && module.name != modules[other].name;
        }
        for (const std::string_view dependency : module.dependencies) {
            if (dependency.empty()) {
                continue;
            }
            const ModuleContract* const target = find_module(modules, dependency);
            // 依赖只能从高层指向低层；严格下降同时排除自依赖、
            // 底层反向包含应用层和任意依赖环。
            valid = valid && target != nullptr && target->layer < module.layer;
        }
        for (std::size_t left = 0U; left < module.dependencies.size(); ++left) {
            if (module.dependencies[left].empty()) {
                continue;
            }
            for (std::size_t right = left + 1U; right < module.dependencies.size(); ++right) {
                valid = valid && module.dependencies[left] != module.dependencies[right];
            }
        }

        std::cout << module.name << " (layer " << module.layer << ")\n  API: "
                  << module.public_interface
                  << "\n  owner: " << module.owner
                  << "\n  invariant: " << module.invariant << '\n';
    }

    const bool service_depends_on_required_interfaces =
        modules[4].dependencies ==
        std::array<std::string_view, 3>{"codec", "algorithms", "executor"};
    valid = valid && has_service && has_executor && service_depends_on_required_interfaces;

    std::cout << "stop: " << project.stop_protocol
              << "\nverification: " << project.verification << '\n';
    return valid ? 0 : 1;
}
