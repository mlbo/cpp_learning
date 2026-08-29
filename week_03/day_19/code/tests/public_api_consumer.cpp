#include "data_structure/heap_demo.h"
#include "data_structure/priority_queue_demo.h"
#include "leetcode/0215_kth_largest/solution.h"
#include "leetcode/0347_top_k_frequent/solution.h"

#include <vector>

int main() {
    heapDemo();
    priorityQueueDemo();

    std::vector<int> values = {3, 2, 1, 5, 6, 4};
    std::vector<int> frequencies = {1, 1, 2};
    leetcode_0215::Solution kth;
    leetcode_0347::Solution top;
    return kth.findKthLargest(values, 2) == 5 && top.topKFrequent(frequencies, 1) ==
                                                    std::vector<int>({1})
               ? 0
               : 1;
}
