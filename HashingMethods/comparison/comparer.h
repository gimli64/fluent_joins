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
using namespace boost::algorithm;

template<class T, class B> class Comparer
{
public:
    Comparer();

    void createTable(result relation, string name, vector<int> keysRepartition, int size = 0);

    void writeTable(T* table);
    T *readTable(string name);

    set<string> binaryJoin(T* table1, T* table2, int primaryPosition, int foreignPosition);
    void compareBuckets(vector<Couple> &values1, vector<Couple> &values2, int primaryPosition, int foreignPosition, set<string> &result);

private:
    string constPrefix;
};

template<class T, class B>
Comparer<T, B>::Comparer()
    :constPrefix("/tmp/tables/")
{
}

template<class T, class B>
void Comparer<T, B>::createTable(result relation, string name, vector<int> keysRepartition, int size)
{
    BucketFactory<B>::getInstance()->reset();
    T table(name, keysRepartition);
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
    cout << "\n\nFinished building table " << name << " : " << BucketFactory<B>::getInstance()->getNumberBuckets() << " buckets" << endl;
//    cout << table << endl;
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
set<string> Comparer<T, B>::binaryJoin(T *table1, T *table2, int primaryPosition, int foreignPosition)
{
    set<string> result;
    vector<Bucket *>::iterator bucket1;
    vector<Bucket *>::iterator bucket2;
    for (size_t keyHash = 0; keyHash < 4; keyHash++) {
        vector<Bucket *> buckets1 = table1->fetchBuckets(keyHash, primaryPosition);
        vector<Bucket *> buckets2 = table2->fetchBuckets(keyHash, foreignPosition);

        for (bucket1 = buckets1.begin(); bucket1 != buckets1.end(); ++bucket1) {
            for (bucket2 = buckets2.begin(); bucket2 != buckets2.end(); ++bucket2) {
                compareBuckets((*bucket1)->getAllValues(), (*bucket2)->getAllValues(), primaryPosition, foreignPosition, result);
            }
        }
    }

    cout << result.size() << endl;
    return result;
}

template<class T, class B>
void Comparer<T, B>::compareBuckets(vector<Couple> &values1, vector<Couple> &values2, int primaryPosition, int foreignPosition, set<string> &result)
{
    vector<Couple>::iterator value1;
    vector<Couple>::iterator value2;
    for (value1 = values1.begin(); value1 != values1.end(); ++value1) {
        for (value2 = values2.begin(); value2 != values2.end(); ++value2) {
            if ((*value1).values[primaryPosition] == (*value2).values[foreignPosition]) {
                result.insert(join((*value1).values, "|") + "$$$" + join((*value2).values, "|"));
            }
        }
    }
}

#endif // COMPARER_H
