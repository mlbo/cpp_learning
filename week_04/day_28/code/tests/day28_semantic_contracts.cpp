#include <iostream>

bool verifyEMCPPReviewContract();

int main() {
    const bool passed = verifyEMCPPReviewContract();
    std::cout << "Day 28 EMC++ semantic contracts: "
              << (passed ? "passed" : "failed") << '\n';
    return passed ? 0 : 1;
}
