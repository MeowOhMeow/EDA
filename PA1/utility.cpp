#include "utility.h"

using namespace std;

vector<string> split(string str, char delim)
{
    vector<string> res;
    string tmp;
    for (auto ch : str)
    {
        if (ch == delim && !tmp.empty())
        {
            res.push_back(tmp);
            tmp.clear();
        }
        else
        {
            tmp.push_back(ch);
        }
    }
    res.push_back(tmp);
    return res;
}
