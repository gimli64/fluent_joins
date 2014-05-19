#ifndef TABLEFACTORY_H
#define TABLEFACTORY_H
#include "extendible/extendibleHashing.h"
#include "common/bucket/bucketFactory.h"
#include "common/bucket/bucket.h"

#include <vector>
#include <fstream>
#include <sys/resource.h>
#include <time.h>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

template<class T, class B> class TableFactory
{
public:
    TableFactory();

    void createTable(result relation, string name, vector<int> keysRepartition, vector<int> interleaveOrder = vector<int>());
    void createAutomatedTable(result relation, string name, vector<int> keysRepartition, vector<int> interleaveOrder = vector<int>());

    void writeTable(T* table);
    T *readTable(string name);

private:
    string constPrefix;
    int insertionLimit;
};

template<class T, class B>
TableFactory<T, B>::TableFactory()
    :constPrefix("/tmp/tables/"), insertionLimit(0)
{
}

template<class T, class B>
void TableFactory<T, B>::createTable(result relation, string name, vector<int> keysRepartition, vector<int> interleaveOrder)
{
    cout << "Building table " << name << endl;
    BucketFactory<B>::getInstance()->reset();
    T table(name, keysRepartition, interleaveOrder);
    clock_t tStart = clock();
    insertionLimit = 160;
    for (int i = 0; i < relation.size(); i++) {
        table.putMultikey(Couple(relation[i][0].c_str(), relation[i]));

        if (i % 10000 == 0)
            cout << "Inserted " << i << " values, time taken :  " <<  (double)(clock() - tStart)/CLOCKS_PER_SEC << "s" << endl;
    }
    cout << "\n\nFinished building table " << name << endl;
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    table.printState();
//    table.splitChainedBuckets();
//    table.printState();
    writeTable(&table);

//    BucketFactory<B>::getInstance()->reset();
//    int j = 0;
//    B* bucket;
//    vector<B*> buckets;
//    for (int i = 0; i < relation.size(); i++) {
//        if (j == 0) {
//            bucket = BucketFactory<B>::getInstance()->newBucket();
//            buckets.push_back(bucket);
//        }
//        bucket->putCouple(Couple(relation[i][0].c_str(), relation[i]));
//        j = (j + 1) % B::BUCKET_SIZE;
//    }
//    BucketFactory<B>::getInstance()->writeAll(buckets, table.getName() + "_aux/");
}

template<class T, class B>
void TableFactory<T, B>::createAutomatedTable(result relation, string name, vector<int> keysRepartition, vector<int> interleaveOrder)
{
    cout << "Building table " << name << endl;
    BucketFactory<B>::getInstance()->reset();
    T table(name, keysRepartition, interleaveOrder);
    clock_t tStart = clock();
    insertionLimit = 160;
    for (int i = 0; i < relation.size(); i++) {
        table.putMultikey(Couple(relation[i][0].c_str(), relation[i]));

        if (i >= insertionLimit) {
            if(table.addBHF()) {
                insertionLimit *= 2;
            } else {
                insertionLimit += (insertionLimit / 10);
            }
        }
        if (i % 10000 == 0)
            cout << "Inserted " << i << " values, time taken :  " <<  (double)(clock() - tStart)/CLOCKS_PER_SEC << "s" << endl;
    }
    cout << "\n\nFinished building table " << name << endl;
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    table.printState();
    writeTable(&table);

//    BucketFactory<B>::getInstance()->reset();
//    int j = 0;
//    B* bucket;
//    vector<B*> buckets;
//    for (int i = 0; i < relation.size(); i++) {
//        if (j == 0) {
//            bucket = BucketFactory<B>::getInstance()->newBucket();
//            buckets.push_back(bucket);
//        }
//        bucket->putCouple(Couple(relation[i][0].c_str(), relation[i]));
//        j = (j + 1) % B::BUCKET_SIZE;
//    }
//    BucketFactory<B>::getInstance()->writeAll(buckets, table.getName() + "_aux/");
}


template<class T, class B>
void TableFactory<T, B>::writeTable(T *table)
{
    ofstream ofs((constPrefix + table->getName() + ".table").c_str());
    {
        text_oarchive oa(ofs);
        oa << *table;
    }
}

template<class T, class B>
T *TableFactory<T, B>::readTable(string name)
{
    T* table = new T();
    {
        ifstream ifs((constPrefix + name + ".table").c_str());
        text_iarchive ia(ifs);
        ia >> *table;
    }
    return table;
}

#endif // TABLEFACTORY_H
