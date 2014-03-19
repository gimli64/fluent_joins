#include "comparer.h"

Comparer::Comparer()
{
}

vector<string> Comparer::fetchData(string inputFile)
{
    vector<string> values;
    ifstream infile(inputFile.c_str());
    for (string line; getline(infile, line); ) {
        values.push_back(line);
    }
    return values;
}
