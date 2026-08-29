#include "cpp11_features/rvalue_reference_demo.h"
#include "data_structure/hash_table_demo.h"
#include "emcpp/item09_type_alias.h"
#include "leetcode/0242_valid_anagram/solution.h"
#include "leetcode/0383_ransom_note/solution.h"

#include "leetcode/0049_group_anagrams/solution.h"
#include "leetcode/0128_longest_consecutive/solution.h"
#include "cpp11_features/universal_reference_demo.h"

#include "cpp11_features/perfect_forwarding_demo.h"
#include "leetcode/0003_longest_substring/solution.h"
#include "leetcode/0438_find_anagrams/solution.h"

#include "data_structure/cache_friendly_demo.h"
#include "data_structure/cpu_cache_demo.h"
#include "data_structure/memory_alignment_demo.h"
#include "leetcode/0005_longest_palindrome/solution.h"
#include "leetcode/0647_palindromic_substrings/solution.h"

#include "leetcode/0076_minimum_window/solution.h"
#include "leetcode/0567_permutation_in_string/solution.h"

#include "leetcode/0146_lru_cache/solution.h"
#include "leetcode/0460_lfu_cache/solution.h"
#include "data_structure/simple_hash_table.h"

#include <type_traits>

static_assert(!std::is_copy_constructible_v<SimpleHashTable>);
static_assert(!std::is_copy_constructible_v<LRUCache>);
static_assert(!std::is_copy_constructible_v<LFUCache>);
static_assert(!std::is_copy_constructible_v<
              hash_table_review::SimpleHashTable<int, int>>);

int main() {
    return 0;
}
