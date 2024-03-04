#include "Solution.h"

int main(int argc, char *argv[])
{
    Assert(argc == 3, "Usage: %s <input file> <output file>", argv[0]);
    Solution solution(argv[1], argv[2]);
    solution.solve();
    solution.save_path();

    return 0;
}