#ifndef COMPARER_H
#define COMPARER_H
#include "extendibleHashing.h"
#include "linearHashing.h"
#include "hybridHashing.h"

#include <vector>
#include <fstream>

using std::ifstream;
using std::string;
using std::getline;
using std::cout; using std::endl;

class Comparer
{
public:
    Comparer();
    vector<string> fetchData(string inputFile);
};

#endif // COMPARER_H
