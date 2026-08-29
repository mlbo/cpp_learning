#include "cpp11_features/lambda_generic_demo.h"
#include "cpp11_features/lambda_init_capture_demo.h"
#include "data_structure/queue_demo.h"
#include "emcpp/item32_item33.h"
#include "leetcode/0225_implement_stack_using_queues/solution.h"
#include "leetcode/0232_implement_queue_using_stacks/solution.h"

int main() {
    queueDemo();
    lambdaGenericDemo();
    lambdaInitCaptureDemo();
    item32Item33Demo();

    MyQueue queue;
    queue.push(1);
    queue.push(2);

    MyStack stack;
    stack.push(1);
    stack.push(2);

    return queue.pop() == 1 && queue.peek() == 2 && stack.pop() == 2 && stack.top() == 1
               ? 0
               : 1;
}
