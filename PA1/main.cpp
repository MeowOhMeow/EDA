#include "Generator.h"
#include "utility.h"
#include "debug.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    Assert(argc == 3, "Usage: %s <input file> <output file>", argv[0]);
    Generator g(argv[1]);
    g.generate();
    cout << "Done!" << endl;
    return 0;
}