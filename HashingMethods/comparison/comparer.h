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

template<class T, class B> class Comparer
{
public:
    Comparer();

    void createTable(result relation, string name, int size = 0);

    void writeTable(T* table);
    T *readTable(string name);

    vector<string> binaryJoin(T* table1, T* table2, int foreignKey);

private:
    string constPrefix;
};

template<class T, class B>
Comparer<T, B>::Comparer()
    :constPrefix("/tmp/tables/")
{
}

template<class T, class B>
void Comparer<T, B>::createTable(result relation, string name, int size)
{
    T table(name);
    if (size > 0) {
        for (int i = 0; i < size; i++) {
            table.put(Couple(relation[i][0].c_str(), relation[i]));
            cout << table << endl;
        }
    } else {
        for (int i = 0; i < relation.size(); i++) {
            table.put(Couple(relation[i][0].c_str(), relation[i]));
        }
    }
    cout << "Finished building table " << name << endl;
    cout << "serializing table " << name << endl;
    BucketFactory<B>::getInstance()->writeAll(table.getBuckets(), table.getBucketPath());
    table.clearBuckets();
    writeTable(&table);
}

template<class T, class B>
void Comparer<T, B>::writeTable(T *table)
{
    ofstream ofs((constPrefix + table->getName() + ".table").c_str());
    {
        text_oarchive oa(ofs);
        oa << *table;
    }
}

template<class T, class B>
T *Comparer<T, B>::readTable(string name)
{
    T* table = new T();
    {
        ifstream ifs((constPrefix + name + ".table").c_str());
        text_iarchive ia(ifs);
        ia >> *table;
    }
    return table;
}

template<class T, class B>
vector<string> Comparer<T, B>::binaryJoin(T *table1, T *table2, int foreignKey)
{

}

#endif // COMPARER_H
