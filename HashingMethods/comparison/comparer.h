#ifndef COMPARER_H
#define COMPARER_H
#include "extendible/extendibleHashing.h"
#include "linear/linearHashing.h"
#include "hybrid/hybridHashing.h"
#include "hybrid/multikeyHybridHashing.h"
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

    void createTable(result relation, string name, vector<int> keysRepartition);

    void writeTable(T* table);
    T *readTable(string name);

    set<string> sortMergeBinaryJoin(T* table1, T* table2, int leftPosition, int rightPosition);
    void mergeCouples(vector<Couple> &couples1, vector<Couple> &couples2, int leftPosition, int rightPosition, set<string> &result);

    set<string> multikeyBinaryJoin(T* table1, T* table2, int leftPosition, int rightPosition);
    void joinBuckets(vector<Couple> &values1, vector<Couple> &values2, int leftPosition, int rightPosition, set<string> &result);

    set<string> multikeyThreeWayJoin(T* table1, T* table2, T* table3, int position1, int position2, int position3);

private:
    string constPrefix;
};

template<class T, class B>
Comparer<T, B>::Comparer()
    :constPrefix("/tmp/tables/")
{
}

template<class T, class B>
void Comparer<T, B>::createTable(result relation, string name, vector<int> keysRepartition)
{
    BucketFactory<B>::getInstance()->reset();
    T table(name, keysRepartition);
    for (int i = 0; i < relation.size(); i++) {
        table.put(Couple(relation[i][0].c_str(), relation[i]));
    }
    cout << table << endl;
    cout << "\n\nFinished building table " << name << " : " << BucketFactory<B>::getInstance()->getNumberBuckets() << " buckets" << endl;
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
set<string> Comparer<T, B>::sortMergeBinaryJoin(T *table1, T *table2, int leftPosition, int rightPosition)
{
    table1->setNumberBucketFetch(0);
    table2->setNumberBucketFetch(0);
    cout << "Using sort merge join" << endl;

    set<string> result;
    vector<Couple> couples1 = table1->getCouples();
    vector<Couple> couples2 = table2->getCouples();

    Comparator comparer(leftPosition);
    sort(couples1.begin(), couples1.end(), comparer);
    comparer = Comparator(rightPosition);
    sort(couples2.begin(), couples2.end(), comparer);
    cout << "Couples successfully sorted" << endl;

    mergeCouples(couples1, couples2, leftPosition, rightPosition, result);
    cout << result.size() << " values successfully joined" << endl;
    return result;
}

template<class T, class B>
void Comparer<T, B>::mergeCouples(vector<Couple> &couples1, vector<Couple> &couples2, int leftPosition, int rightPosition, set<string> &result)
{
    vector<Couple>::iterator couple1 = couples1.begin();
    vector<Couple>::iterator couple2 = couples2.begin();

    while (couple1 != couples1.end() and couple2 != couples2.end()) {
        if ((*couple1).values[leftPosition] < (*couple2).values[rightPosition]) {
            couple1 = next(couple1);
        } else if ((*couple1).values[leftPosition] > (*couple2).values[rightPosition]) {
            couple2 = next(couple2);
        } else {
            result.insert(join((*couple1).values, "|") + "$$$" + join((*couple2).values, "|"));

            vector<Couple>::iterator couple1_temp = next(couple1);
            while (couple1_temp != couples1.end() and (*couple1_temp).values[leftPosition] == (*couple2).values[rightPosition]) {
                result.insert(join((*couple1_temp).values, "|") + "$$$" + join((*couple2).values, "|"));
                couple1_temp = next(couple1_temp);
            }

            vector<Couple>::iterator couple2_temp = next(couple2);
            while (couple2_temp != couples2.end() and (*couple1).values[leftPosition] == (*couple2_temp).values[rightPosition]) {
                result.insert(join((*couple1).values, "|") + "$$$" + join((*couple2_temp).values, "|"));
                couple2_temp = next(couple2_temp);
            }

            couple1 = next(couple1);
            couple2 = next(couple2);
        }
    }
}

template<class T, class B>
set<string> Comparer<T, B>::multikeyBinaryJoin(T *table1, T *table2, int leftPosition, int rightPosition)
{
    table1->setNumberBucketFetch(0);
    table2->setNumberBucketFetch(0);
    cout << "Using multikeyBinaryJoin" << endl;

    set<string> result;
    vector<Bucket *>::iterator bucket1;
    vector<Bucket *>::iterator bucket2;
    int keyHashSize = min(table1->keysRepartition[leftPosition], table2->keysRepartition[rightPosition]);
    for (size_t keyHash = 0; keyHash < (int) pow(2.0, (double) keyHashSize); keyHash++) {
        vector<Bucket *> buckets1 = table1->fetchBuckets(keyHash, keyHashSize, leftPosition);
        vector<Bucket *> buckets2 = table2->fetchBuckets(keyHash, keyHashSize, rightPosition);

        for (bucket1 = buckets1.begin(); bucket1 != buckets1.end(); ++bucket1) {
            for (bucket2 = buckets2.begin(); bucket2 != buckets2.end(); ++bucket2) {
                joinBuckets((*bucket1)->getAllValues(), (*bucket2)->getAllValues(), leftPosition, rightPosition, result);
            }
        }
    }

    cout << result.size() << " values successfully joined" << endl;
    return result;
}

template<class T, class B>
set<string> Comparer<T, B>::multikeyThreeWayJoin(T *table1, T *table2, T *table3, int position1, int position2, int position3)
{
    table1->setNumberBucketFetch(0);
    table2->setNumberBucketFetch(0);
    cout << "Using multikeyThreeWayJoin" << endl;

    set<string> result;
    vector<Bucket *>::iterator bucket1;
    vector<Bucket *>::iterator bucket2;
    vector<Bucket *>::iterator bucket3;
    int key_1_2_size = min(table1->keysRepartition[position1], table2->keysRepartition[position2]);
    int key_2_3_size = min(table2->keysRepartition[position2], table3->keysRepartition[position3]);
    for (size_t key_1_2_hash = 0; key_1_2_hash < (int) pow(2.0, (double) key_1_2_size); key_1_2_hash++) {
        vector<Bucket *> buckets1 = table1->fetchBuckets(key_1_2_hash, key_1_2_size, position1);
    }
}

template<class T, class B>
void Comparer<T, B>::joinBuckets(vector<Couple> &values1, vector<Couple> &values2, int leftPosition, int rightPosition, set<string> &result)
{
    vector<Couple>::iterator value1;
    vector<Couple>::iterator value2;
    for (value1 = values1.begin(); value1 != values1.end(); ++value1) {
        for (value2 = values2.begin(); value2 != values2.end(); ++value2) {
            if ((*value1).values[leftPosition] == (*value2).values[rightPosition]) {
                result.insert(join((*value1).values, "|") + "$$$" + join((*value2).values, "|"));
            }
        }
    }
}

#endif // COMPARER_H
