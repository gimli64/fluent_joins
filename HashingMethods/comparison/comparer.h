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

template<class T> class Comparer
{
public:
    Comparer();

    void writeTable(T* table);
    T *readTable(string name);

private:
    string constPrefix;
};

template<class T>
Comparer<T>::Comparer()
    :constPrefix("/tmp/tables/")
{
}

template<class T>
void Comparer<T>::writeTable(T *table)
{
    ofstream ofs((constPrefix + table->getName() + ".table").c_str());
    {
        text_oarchive oa(ofs);
        oa << *table;
    }
}

template<class T>
T *Comparer<T>::readTable(string name)
{
    T* table = new T();
    {
        ifstream ifs((constPrefix + name + ".table").c_str());
        text_iarchive ia(ifs);
        ia >> *table;
    }
    return table;
}

#endif // COMPARER_H
