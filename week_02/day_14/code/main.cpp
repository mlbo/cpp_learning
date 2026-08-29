/**
 * @file main.cpp
 * @brief Day 14: 第二周复习与综合练习 - 主程序
 *
 * 本程序整合：
 * 1. 第二周知识点总结
 * 2. EMC++条款17-22复习
 * 3. 综合练习题
 * 4. 线程安全链表项目
 * 5. LeetCode 234 & 138 题解演示
 */

#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>
#include "project/thread_safe_list_demo.h"
#include "review/emcpp_review.h"
#include "review/exercises.h"
#include "review/week2_summary.h"

// LeetCode 模块

// ============================================================
// LeetCode 234 回文链表演示
// ============================================================

void demo_leetcode_234() {
    std::cout << "\n=== LeetCode 234: 回文链表演示 ===\n";
    std::cout << "请运行独立测试程序: test_leetcode234\n";
}

// ============================================================
// LeetCode 138 随机链表复制演示
// ============================================================

void demo_leetcode_138() {
    std::cout << "\n=== LeetCode 138: 随机链表复制演示 ===\n";
    std::cout << "请运行独立测试程序: test_leetcode138\n";
}

// ============================================================
// 主菜单
// ============================================================

void print_menu(std::ostream& output) {
    output << "\n";
    output << "╔══════════════════════════════════════════════════════╗\n";
    output << "║        Day 14: 第二周复习与综合练习                     ║\n";
    output << "╠══════════════════════════════════════════════════════╣\n";
    output << "║  1. 第二周知识点总结                                   ║\n";
    output << "║  2. EMC++条款17-22复习                                 ║\n";
    output << "║  3. 综合练习题                                         ║\n";
    output << "║  4. 线程安全链表演示                                   ║\n";
    output << "║  5. LeetCode 234: 回文链表                            ║\n";
    output << "║  6. LeetCode 138: 随机链表复制                        ║\n";
    output << "║  7. 运行全部演示                                       ║\n";
    output << "║  0. 退出                                               ║\n";
    output << "╚══════════════════════════════════════════════════════╝\n";
    output << "请选择 [0-7]: ";
}

void run_all() {
    std::cout << "\n╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║             运行全部演示                               ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";

    week2::run_week2_summary();
    emcpp::run_emcpp_review();
    exercises::run_exercises();
    week2_concurrency::demo::run();
    demo_leetcode_234();
    demo_leetcode_138();

    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════════════════╗\n";
    std::cout << "║            恭喜完成第二周复习！                         ║\n";
    std::cout << "╚══════════════════════════════════════════════════════╝\n";
}

namespace {

enum class MenuInputStatus {
    choice,
    invalid,
    end_of_input,
    stream_error
};

MenuInputStatus classify_read_failure(const std::istream& input) noexcept {
    if (input.bad()) {
        return MenuInputStatus::stream_error;
    }
    if (input.eof()) {
        return MenuInputStatus::end_of_input;
    }
    return MenuInputStatus::stream_error;
}

MenuInputStatus read_menu_choice(std::istream& input, int& choice) {
    std::string line;
    try {
        if (!std::getline(input, line)) {
            return classify_read_failure(input);
        }
    } catch (const std::ios_base::failure&) {
        return classify_read_failure(input);
    }

    std::istringstream parser(line);
    int parsed = 0;
    if (!(parser >> parsed)) {
        return MenuInputStatus::invalid;
    }
    parser >> std::ws;
    if (!parser.eof()) {
        return MenuInputStatus::invalid;
    }

    choice = parsed;
    return MenuInputStatus::choice;
}

class FailingInputBuffer : public std::streambuf {
protected:
    int_type underflow() override {
        throw std::ios_base::failure("injected input failure");
    }
};

int run_menu(std::istream& input, std::ostream& output) {
    while (true) {
        print_menu(output);

        int choice = 0;
        const MenuInputStatus status = read_menu_choice(input, choice);
        if (status == MenuInputStatus::end_of_input) {
            output << "\n输入结束，退出菜单。\n";
            return 0;
        }
        if (status == MenuInputStatus::stream_error) {
            output << "\n读取输入失败，退出菜单。\n";
            return 1;
        }
        if (status == MenuInputStatus::invalid) {
            output << "请输入0到7之间的整数，且一行只输入一个选项。\n";
            continue;
        }

        switch (choice) {
            case 1:
                week2::run_week2_summary();
                break;
            case 2:
                emcpp::run_emcpp_review();
                break;
            case 3:
                exercises::run_exercises();
                break;
            case 4:
                week2_concurrency::demo::run();
                break;
            case 5:
                demo_leetcode_234();
                break;
            case 6:
                demo_leetcode_138();
                break;
            case 7:
                run_all();
                break;
            case 0:
                output << "\n感谢使用，再见！\n";
                return 0;
            default:
                output << "无效选择，请重试。\n";
        }
    }
}

bool run_menu_contract_tests() {
    bool ok = true;

    {
        std::istringstream input("3\n");
        int choice = -1;
        ok = ok && read_menu_choice(input, choice) == MenuInputStatus::choice;
        ok = ok && choice == 3;
    }
    {
        std::istringstream input("3 trailing\n");
        int choice = 99;
        ok = ok && read_menu_choice(input, choice) == MenuInputStatus::invalid;
        ok = ok && choice == 99;
    }
    {
        std::istringstream input("bad\n3 trailing\n0\n");
        std::ostringstream output;
        ok = ok && run_menu(input, output) == 0;
        const std::string text = output.str();
        const std::string rejection = "一行只输入一个选项";
        const std::size_t first_rejection = text.find(rejection);
        ok = ok && first_rejection != std::string::npos;
        ok = ok && text.find(rejection, first_rejection + rejection.size()) !=
                       std::string::npos;
        ok = ok && output.str().find("感谢使用") != std::string::npos;
    }
    {
        std::istringstream input;
        std::ostringstream output;
        ok = ok && run_menu(input, output) == 0;
        ok = ok && output.str().find("输入结束") != std::string::npos;
    }
    {
        std::istringstream input("0\n");
        input.setstate(std::ios::badbit | std::ios::eofbit);
        std::ostringstream output;
        ok = ok && run_menu(input, output) == 1;
        ok = ok && output.str().find("读取输入失败") != std::string::npos;
    }
    {
        std::istringstream input("0\n");
        input.setstate(std::ios::failbit);
        std::ostringstream output;
        ok = ok && run_menu(input, output) == 1;
        ok = ok && output.str().find("读取输入失败") != std::string::npos;
    }
    {
        std::istringstream input;
        input.exceptions(std::ios::failbit | std::ios::badbit);
        std::ostringstream output;
        ok = ok && run_menu(input, output) == 0;
        ok = ok && output.str().find("输入结束") != std::string::npos;
    }
    {
        FailingInputBuffer buffer;
        std::istream input(&buffer);
        input.exceptions(std::ios::badbit);
        std::ostringstream output;
        ok = ok && run_menu(input, output) == 1;
        ok = ok && output.str().find("读取输入失败") != std::string::npos;
    }

    return ok;
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc > 1) {
        const std::string option(argv[1]);
        if (option == "--all") {
            run_all();
            return 0;
        }
        if (option == "--menu-contracts") {
            return run_menu_contract_tests() ? 0 : 1;
        }
        std::cerr << "未知参数: " << option
                  << "（可用参数: --all, --menu-contracts）\n";
        return 2;
    }

    return run_menu(std::cin, std::cout);
}
