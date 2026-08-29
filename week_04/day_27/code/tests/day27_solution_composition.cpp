#include "leetcode/0076_minimum_window/solution.h"
#include "leetcode/0567_permutation_in_string/solution.h"

int main() {
    day27::lc0076::Solution minimumWindow;
    day27::lc0567::Solution permutation;

    const bool minimumWindowWorks =
        minimumWindow.minWindow("ADOBECODEBANC", "ABC") == "BANC";
    const bool permutationWorks =
        permutation.checkInclusion("ab", "eidbaooo");

    return minimumWindowWorks && permutationWorks ? 0 : 1;
}
