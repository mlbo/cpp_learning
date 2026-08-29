#include "common/noexcept_output.h"
#include "day_08/code/data_structure/list_node.h"
#include "day_08/code/leetcode/0203_remove_elements/solution.h"
#include "day_08/code/leetcode/0206_reverse_list/solution.h"
#include "day_09/code/leetcode/0021_merge_lists/solution.h"
#include "day_09/code/leetcode/0141_cycle/solution.h"
#include "day_10/code/leetcode/0142_cycle_entrance/solution.h"
#include "day_11/code/cpp11_features/pimpl_widget.h"
#include "day_11/code/cpp11_features/pimpl_widget_impl.h"
#include "day_11/code/leetcode/0023_merge_k_lists/solution.h"
#include "day_11/code/leetcode/0061_rotate_list/solution.h"
#include "day_12/code/leetcode/0024_swap_pairs/solution.h"
#include "day_13/code/leetcode/0148_sort_list/solution.h"
#include "day_13/code/leetcode/0160_intersection/solution.h"
#include "day_13/code/list_node.h"
#include "day_14/code/leetcode/0138_copy_random/solution.h"
#include "day_14/code/leetcode/0234_palindrome/solution.h"
#include "day_14/code/project/thread_safe_list.h"
#include "day_14/code/project/thread_safe_list_demo.h"
#include "day_14/code/review/emcpp_review.h"
#include "day_14/code/review/exercises.h"
#include "day_14/code/review/week2_summary.h"

#include <type_traits>

static_assert(!std::is_same_v<day08_lists::ListNode, day13_lists::ListNode>);
static_assert(!std::is_same_v<leetcode_0021::ListNode, leetcode_0141::ListNode>);
static_assert(!std::is_same_v<leetcode_0148::ListNode, leetcode_0160::ListNode>);
static_assert(!std::is_same_v<leetcode_0148::ListNode, leetcode_0234::ListNode>);
static_assert(!std::is_same_v<leetcode_0138::Node, leetcode_0234::ListNode>);

int main() {
    week2_concurrency::ThreadSafeList<int> list;
    list.push_front(1);
    return list.size() == 1U ? 0 : 1;
}
