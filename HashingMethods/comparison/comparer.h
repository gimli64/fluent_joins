#ifndef COMPARER_H
#define COMPARER_H
#include "extendible/extendibleHashing.h"
#include "linear/linearHashing.h"
#include "hybrid/hybridHashing.h"
#include "common/bucket/bucketFactory.h"

#include <vector>
#include <fstream>
#include <sys/resource.h>

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
