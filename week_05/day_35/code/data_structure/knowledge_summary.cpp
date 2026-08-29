#include <array>
#include <iostream>
#include <string_view>

struct ComplexityCard {
    std::string_view structure;
    std::string_view strength;
    std::string_view boundary;
};

int main() {
    constexpr std::array<ComplexityCard, 7> cards{{
        {"vector", "O(1) random access", "middle insertion moves elements"},
        {"list", "O(1) relink with a known position and predecessor",
         "locating a position is O(n) and cache locality is weak"},
        {"stack/queue", "restricted O(1) end operations", "no arbitrary access contract"},
        {"unordered_map", "average O(1) lookup", "worst case is O(n)"},
        {"BST", "operations are O(h) for tree height h",
         "balanced h is O(log n); unbalanced worst case is O(n)"},
        {"tree DFS", "O(n) traversal", "recursive/explicit stack is O(h)"},
        {"tree BFS", "O(n) traversal", "queue is O(w) for maximum tree width w"},
    }};

    bool all_fields_present = true;
    bool list_precondition_present = false;
    bool bst_height_present = false;
    bool traversal_space_split = false;
    for (const ComplexityCard& card : cards) {
        all_fields_present = all_fields_present && !card.structure.empty() &&
                             !card.strength.empty() && !card.boundary.empty();
        list_precondition_present = list_precondition_present ||
                                    (card.structure == "list" &&
                                     card.strength.find("known position") != std::string_view::npos);
        bst_height_present = bst_height_present ||
                             (card.structure == "BST" &&
                              card.strength.find("O(h)") != std::string_view::npos &&
                              card.boundary.find("O(n)") != std::string_view::npos);
        traversal_space_split = traversal_space_split ||
                                (card.structure == "tree BFS" &&
                                 card.boundary.find("O(w)") != std::string_view::npos);
        std::cout << card.structure << ": " << card.strength
                  << "; boundary: " << card.boundary << '\n';
    }
    return all_fields_present && list_precondition_present && bst_height_present &&
                   traversal_space_split
               ? 0
               : 1;
}
