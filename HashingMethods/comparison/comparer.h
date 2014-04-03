#ifndef COMPARER_H
#define COMPARER_H
#include "extendible/extendibleHashing.h"
#include "linear/linearHashing.h"
#include "hybrid/hybridHashing.h"
#include "common/bucket/bucketFactory.h"
#include "common/bucket/bucket.h"

#include <vector>
#include <fstream>
#include <sys/resource.h>
#include <boost/algorithm/string/join.hpp>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

class Comparer
{
public:
    Comparer();
    vector<string> fetchData(string inputFile);
};

#endif // COMPARER_H
