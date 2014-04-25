#ifndef COMPARER_H
#define COMPARER_H
#include "extendible/extendibleHashing.h"
#include "linear/linearHashing.h"
#include "hybrid/hybridHashing.h"
#include "hybrid/multikeyHybridHashing.h"
#include "extendible/multikeyExtendibleHashing.h"
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

    set<string> sortMergeThreeWayJoin(T *table1, T *table2, T *table3, int position1, int position1_2, int position2_3, int position3);
    void threeWayMergeCouples(vector<Couple> &couples1, vector<Couple> &couples2, int leftPosition, int rightPosition, vector<Couple> &interCouples);

    set<string> multikeyBinaryJoin(T* table1, T* table2, int leftPosition, int rightPosition);
    void binaryJoinCouples(vector<Couple> &values1, vector<Couple> &values2, int leftPosition, int rightPosition, set<string> &result);

    set<string> multikeyThreeWayJoin(T* table1, T* table2, T* table3, int position1, int position1_2, int position2_3, int position3);
    void threeWayJoinCouples(vector<Couple> &couples1, vector<Couple> &couples2, vector<Couple> &couples3, int position1, int position1_2, int position2_3, int position3, set<string> &result);

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
        table.putMultikey(Couple(relation[i][0].c_str(), relation[i]));
    }
    //    cout << table << endl;
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

    Comparator comparator(leftPosition);
    sort(couples1.begin(), couples1.end(), comparator);
    comparator = Comparator(rightPosition);
    sort(couples2.begin(), couples2.end(), comparator);

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
set<string> Comparer<T, B>::sortMergeThreeWayJoin(T *table1, T *table2, T *table3, int position1, int position1_2, int position2_3, int position3)
{
    table1->setNumberBucketFetch(0);
    table2->setNumberBucketFetch(0);
    table3->setNumberBucketFetch(0);
    cout << "Using sort merge join" << endl;

    set<string> result;
    vector<Couple> couples1 = table1->getCouples();
    vector<Couple> couples2 = table2->getCouples();
    vector<Couple> interCouples = vector<Couple>();
    vector<Couple> couples3 = table3->getCouples();

    Comparator comparator(position1);
    sort(couples1.begin(), couples1.end(), comparator);
    comparator = Comparator(position1_2);
    sort(couples2.begin(), couples2.end(), comparator);
    threeWayMergeCouples(couples2, couples1, position1_2, position1, interCouples);

    comparator = Comparator(position2_3);
    sort(interCouples.begin(), interCouples.end(), comparator);
    comparator = Comparator(position3);
    sort(couples3.begin(), couples3.end(), comparator);
    mergeCouples(interCouples, couples3, position2_3, position3, result);

    cout << result.size() << " values successfully joined" << endl;
    return result;
}

template<class T, class B>
void Comparer<T, B>::threeWayMergeCouples(vector<Couple> &couples1, vector<Couple> &couples2, int leftPosition, int rightPosition, vector<Couple> &interCouples)
{
    vector<Couple>::iterator couple1 = couples1.begin();
    vector<Couple>::iterator couple2 = couples2.begin();

    while (couple1 != couples1.end() and couple2 != couples2.end()) {
        if ((*couple1).values[leftPosition] < (*couple2).values[rightPosition]) {
            couple1 = next(couple1);
        } else if ((*couple1).values[leftPosition] > (*couple2).values[rightPosition]) {
            couple2 = next(couple2);
        } else {
            Couple couple = Couple();
            couple.values = (*couple1).values;
            couple.values.insert(couple.values.end(), (*couple2).values.begin(), (*couple2).values.end());
            interCouples.push_back(couple);

            vector<Couple>::iterator couple1_temp = next(couple1);
            while (couple1_temp != couples1.end() and (*couple1_temp).values[leftPosition] == (*couple2).values[rightPosition]) {
                Couple couple = Couple();
                couple.values = (*couple1_temp).values;
                couple.values.insert(couple.values.end(), (*couple2).values.begin(), (*couple2).values.end());
                interCouples.push_back(couple);
                couple1_temp = next(couple1_temp);
            }

            vector<Couple>::iterator couple2_temp = next(couple2);
            while (couple2_temp != couples2.end() and (*couple1).values[leftPosition] == (*couple2_temp).values[rightPosition]) {
                Couple couple = Couple();
                couple.values = (*couple1).values;
                couple.values.insert(couple.values.end(), (*couple2_temp).values.begin(), (*couple2_temp).values.end());
                interCouples.push_back(couple);
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
        vector<Couple> couples1 = table1->fetchCouples(keyHash, keyHashSize, leftPosition);
        vector<Couple> couples2 = table2->fetchCouples(keyHash, keyHashSize, rightPosition);

        binaryJoinCouples(couples1, couples2, leftPosition, rightPosition, result);
    }

    cout << result.size() << " values successfully joined" << endl;
    return result;
}

template<class T, class B>
void Comparer<T, B>::binaryJoinCouples(vector<Couple> &couples1, vector<Couple> &couples2, int leftPosition, int rightPosition, set<string> &result)
{
    vector<Couple>::iterator couple1;
    vector<Couple>::iterator couple2;
    for (couple1 = couples1.begin(); couple1 != couples1.end(); ++couple1) {
        for (couple2 = couples2.begin(); couple2 != couples2.end(); ++couple2) {
            if ((*couple1).values[leftPosition] == (*couple2).values[rightPosition]) {
                result.insert(join((*couple1).values, "|") + "$$$" + join((*couple2).values, "|"));
            }
        }
    }
}

template<class T, class B>
set<string> Comparer<T, B>::multikeyThreeWayJoin(T *table1, T *table2, T *table3, int position1, int position1_2, int position2_3, int position3)
{
    table1->setNumberBucketFetch(0);
    table2->setNumberBucketFetch(0);
    table3->setNumberBucketFetch(0);
    cout << "Using multikeyThreeWayJoin" << endl;

    set<string> result;
    vector<Bucket *>::iterator bucket1;
    vector<Bucket *>::iterator bucket2;
    vector<Bucket *>::iterator bucket3;
    int key_1_2_size = min(table1->keysRepartition[position1], table2->keysRepartition[position1_2]);
    int key_2_3_size = min(table2->keysRepartition[position2_3], table3->keysRepartition[position3]);
    for (size_t key_1_2_hash = 0; key_1_2_hash < (int) pow(2.0, (double) key_1_2_size); key_1_2_hash++) {
        vector<Couple> couples1 = table1->fetchCouples(key_1_2_hash, key_1_2_size, position1);

        for (size_t key_2_3_hash = 0; key_2_3_hash < (int) pow(2.0, (double) key_2_3_size); key_2_3_hash++) {
            vector<Couple> couples2 = table2->fetchCouples(key_1_2_hash, key_1_2_size, position1_2, key_2_3_hash, key_2_3_size, position2_3);
            vector<Couple> couples3 = table3->fetchCouples(key_2_3_hash, key_2_3_size, position3);

            threeWayJoinCouples(couples3, couples2, couples1, position3, position2_3, position1_2, position1, result);
        }
    }

    cout << result.size() << " values successfully joined" << endl;
    return result;
}

template<class T, class B>
void Comparer<T, B>::threeWayJoinCouples(vector<Couple> &couples1, vector<Couple> &couples2, vector<Couple> &couples3, int position1, int position1_2, int position2_3, int position3, set<string> &result)
{
    vector<Couple>::iterator couple1;
    vector<Couple>::iterator couple2;
    vector<Couple>::iterator couple3;
    for (couple1 = couples1.begin(); couple1 != couples1.end(); ++couple1) {
        for (couple2 = couples2.begin(); couple2 != couples2.end(); ++couple2) {
            if ((*couple1).values[position1] == (*couple2).values[position1_2]) {
                for (couple3 = couples3.begin(); couple3 != couples3.end(); ++couple3) {
                    if ((*couple2).values[position2_3] == (*couple3).values[position3]) {
                        result.insert(join((*couple1).values, "|") + "$$$" + join((*couple2).values, "|") +  "$$$" + join((*couple3).values, "|"));
                    }
                }
            }
        }
    }
}

#endif // COMPARER_H
