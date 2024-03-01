#include "Generator.h"
#include "utility.h"
#include "debug.h"
#include <iostream>
#include <cstdlib>
#include <string>

#ifdef _WIN32
string openCommand = "start ";
#else
string openCommand = "xdg-open ";
#endif

using namespace std;

int main(int argc, char *argv[])
{
    string url = "https://youtu.be/dQw4w9WgXcQ?si=kKf0nsl9uuBP3TZk";
    system((openCommand + url).c_str());

    Assert(argc == 3, "Usage: %s <input file> <output file>", argv[0]);
    Generator g(argv[1]);
    g.generate();
    clog << "Done!" << endl;
    return 0;
}