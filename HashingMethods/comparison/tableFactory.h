#ifndef TABLEFACTORY_H
#define TABLEFACTORY_H
//#include "extendible/extendibleHashing.h"
//#include "hybrid/multikeyHybridHashing.h"
#include "extendible/multikeyExtendibleHashing.h"
//#include "linear/multikeyLinearHashing.h"
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

    void createTable(result relation, string name, vector<int> keysRepartition);

    void writeTable(T* table);
    T *readTable(string name);

private:
    string constPrefix;
    int insertionLimit;
};

template<class T, class B>
TableFactory<T, B>::TableFactory()
    :constPrefix("/tmp/tables/"), insertionLimit(40)
{
}

template<class T, class B>
void TableFactory<T, B>::createTable(result relation, string name, vector<int> keysRepartition)
{
    BucketFactory<B>::getInstance()->reset();
    T table(name, keysRepartition);
    clock_t tStart = clock();
    for (int i = 0; i < relation.size(); i++) {
        table.putMultikey(Couple(relation[i][0].c_str(), relation[i]));

        if (i >= insertionLimit - 1 or i >= relation.size() - 1) {
            cout << "\nInserted " << (i + 1) << " values, " << "checking table structure" << endl;
            table.checkStructure();
            double numberUpdates = table.updateStructure();
            if (numberUpdates > 0) {
                table.reInsertCouples();
                insertionLimit = int(insertionLimit * (2 * numberUpdates));
            } else {
                insertionLimit += 10;
            }
        }
    }
//    cout << table << endl;
    cout << "\n\nFinished building multikey table " << name << endl;
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    BucketFactory<B>::getInstance()->writeAll(table.getBuckets(), table.getBucketPath());
    table.clearBuckets();
    table.printState();
    writeTable(&table);

    cout << "\nBuilding standard table" << endl;
    BucketFactory<B>::getInstance()->reset();
    int j = 0;
    B* bucket;
    vector<B*> buckets;
    for (int i = 0; i < relation.size(); i++) {
        if (j == 0) {
            bucket = BucketFactory<B>::getInstance()->newBucket();
            buckets.push_back(bucket);
        }
        bucket->putCouple(Couple(relation[i][0].c_str(), relation[i]));
        j = (j + 1) % B::BUCKET_SIZE;
    }
    BucketFactory<B>::getInstance()->writeAll(buckets, table.getName() + "_aux/");
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
