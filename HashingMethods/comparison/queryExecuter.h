#ifndef QUERYEXECUTER_H
#define QUERYEXECUTER_H

#include "extendible/extendibleHashing.h"
#include "common/bucket/bucketFactory.h"
#include "common/bucket/bucket.h"
#include <boost/algorithm/string/join.hpp>
#include <time.h>

using namespace boost::algorithm;


template<class T, class B> class QueryExecuter
{
public:
    QueryExecuter();
    vector<Couple> readStandardTable(string name, int numberEntries);

    vector<string> sortMergeBinaryJoin(T *table1, T *table2, int leftPosition, int rightPosition);
    void mergeCouples(vector<Couple> &couples1, vector<Couple> &couples2, int leftPosition, int rightPosition, vector<string> &result);

    vector<string> sortMergeThreeWayJoin(T *table1, T *table2, T *table3, int position1, int position1_2, int position2_3, int position3);
    void threeWayMergeCouples(vector<Couple> &couples1, vector<Couple> &couples2, int leftPosition, int rightPosition, vector<Couple> &interCouples);

    vector<string> multikeyBinaryJoin(T* table1, T* table2, int leftPosition, int rightPosition);
    void binaryJoinCouples(vector<Couple> &values1, vector<Couple> &values2, int leftPosition, int rightPosition, vector<string> &result);

    vector<string> multikeyThreeWayJoin(T* table1, T* table2, T* table3, int position1, int position1_2, int position2_3, int position3);
    void threeWayJoinCouples(vector<Couple> &couples1, vector<Couple> &couples2, vector<Couple> &couples3, int position1, int position1_2, int position2_3, int position3, vector<string> &result);
};

template<class T, class B>
QueryExecuter<T, B>::QueryExecuter()
{
}

template<class T, class B>
vector<Couple> QueryExecuter<T, B>::readStandardTable(string name, int numberEntries)
{
    int numberBuckets = (int) ceil((double) numberEntries / B::BUCKET_SIZE);
    B *bucket;
    vector<Couple> couples;
    for (int i = 0; i < numberBuckets; i++) {
        bucket = BucketFactory<B>::getInstance()->readBucket(name + "_aux/b" + lexical_cast<string>(i));
        vector<Couple> values = bucket->getAllValues();
        couples.insert(couples.end(), values.begin(), values.end());
    }

    return couples;
}

template<class T, class B>
vector<string> QueryExecuter<T, B>::sortMergeBinaryJoin(T *table1, T *table2, int leftPosition, int rightPosition)
{
    cout << "Using sort merge join" << endl;
    table1->setNumberBucketFetch(0);
    table1->reset();
    table2->setNumberBucketFetch(0);
    table2->reset();
    vector<string> result;
    vector<Couple> couples1 = table1->fetchAllCouples();
    vector<Couple> couples2 = table2->fetchAllCouples();

    Comparator comparator(leftPosition);
    sort(couples1.begin(), couples1.end(), comparator);
    comparator = Comparator(rightPosition);
    sort(couples2.begin(), couples2.end(), comparator);

    mergeCouples(couples1, couples2, leftPosition, rightPosition, result);
    cout << result.size() << " values successfully joined" << endl;
    cout << "table " << table1->getName() << " : " << table1->getNumberBucketFetch() << " bucket fetch" << endl;
    cout << "table " << table2->getName() << " : " << table2->getNumberBucketFetch() << " bucket fetch" << endl;
    return result;
}

template<class T, class B>
void QueryExecuter<T, B>::mergeCouples(vector<Couple> &couples1, vector<Couple> &couples2, int leftPosition, int rightPosition, vector<string> &result)
{
    vector<Couple>::iterator couple1 = couples1.begin();
    vector<Couple>::iterator couple2 = couples2.begin();

    while (couple1 != couples1.end() and couple2 != couples2.end()) {
        if ((*couple1).values[leftPosition] < (*couple2).values[rightPosition]) {
            couple1 = next(couple1);
        } else if ((*couple1).values[leftPosition] > (*couple2).values[rightPosition]) {
            couple2 = next(couple2);
        } else {
            result.push_back(join((*couple1).values, "|") + "$$$" + join((*couple2).values, "|"));

            vector<Couple>::iterator couple1_temp = next(couple1);
            while (couple1_temp != couples1.end() and (*couple1_temp).values[leftPosition] == (*couple2).values[rightPosition]) {
                result.push_back(join((*couple1_temp).values, "|") + "$$$" + join((*couple2).values, "|"));
                couple1_temp = next(couple1_temp);
            }

            vector<Couple>::iterator couple2_temp = next(couple2);
            while (couple2_temp != couples2.end() and (*couple1).values[leftPosition] == (*couple2_temp).values[rightPosition]) {
                result.push_back(join((*couple1).values, "|") + "$$$" + join((*couple2_temp).values, "|"));
                couple2_temp = next(couple2_temp);
            }

            couple1 = next(couple1);
            couple2 = next(couple2);
        }
    }
}

template<class T, class B>
vector<string> QueryExecuter<T, B>::sortMergeThreeWayJoin(T *table1, T *table2, T *table3, int position1, int position1_2, int position2_3, int position3)
{
    cout << "Using sort merge join" << endl;

    table1->setNumberBucketFetch(0);
    table1->reset();
    table2->setNumberBucketFetch(0);
    table2->reset();
    table3->setNumberBucketFetch(0);
    table3->reset();
    vector<string> result;
    vector<Couple> couples1 = table1->fetchAllCouples();
    vector<Couple> couples2 = table2->fetchAllCouples();
    vector<Couple> interCouples = vector<Couple>();
    vector<Couple> couples3 = table3->fetchAllCouples();

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
    cout << "table " << table1->getName() << " : " << table1->getNumberBucketFetch() << " bucket fetch" << endl;
    cout << "table " << table2->getName() << " : " << table2->getNumberBucketFetch() << " bucket fetch" << endl;
    cout << "table " << table3->getName() << " : " << table3->getNumberBucketFetch() << " bucket fetch" << endl;
    return result;
}

template<class T, class B>
void QueryExecuter<T, B>::threeWayMergeCouples(vector<Couple> &couples1, vector<Couple> &couples2, int leftPosition, int rightPosition, vector<Couple> &interCouples)
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
vector<string> QueryExecuter<T, B>::multikeyBinaryJoin(T *table1, T *table2, int leftPosition, int rightPosition)
{
    table1->setNumberBucketFetch(0);
    table1->reset();
    table2->setNumberBucketFetch(0);
    table2->reset();
    cout << "Using multikeyBinaryJoin" << endl;

    vector<string> result;
    vector<Couple> couples1;
    vector<Couple> couples2;
    int keyHashSize = min(table1->keysRepartition[leftPosition], table2->keysRepartition[rightPosition]);
    cout << "key hash size : " << keyHashSize << endl;
    for (size_t keyHash = 0; keyHash < (int) pow(2.0, (double) keyHashSize); keyHash++) {
        table1->reset();
        table2->reset();
        couples1 = table1->fetchCouples(keyHash, keyHashSize, leftPosition);
        couples2 = table2->fetchCouples(keyHash, keyHashSize, rightPosition);
        binaryJoinCouples(couples1, couples2, leftPosition, rightPosition, result);
        cout << result.size() << " values joined so far." << endl;
    }

    cout << result.size() << " values successfully joined" << endl;
    cout << "table " << table1->getName() << " : " << table1->getNumberBucketFetch() << " bucket fetch" << endl;
    cout << "table " << table2->getName() << " : " << table2->getNumberBucketFetch() << " bucket fetch" << endl;
    return result;
}

template<class T, class B>
void QueryExecuter<T, B>::binaryJoinCouples(vector<Couple> &couples1, vector<Couple> &couples2, int leftPosition, int rightPosition, vector<string> &result)
{
    vector<Couple>::iterator couple1;
    vector<Couple>::iterator couple2;
    for (couple1 = couples1.begin(); couple1 != couples1.end(); ++couple1) {
        for (couple2 = couples2.begin(); couple2 != couples2.end(); ++couple2) {
            if ((*couple1).values[leftPosition] == (*couple2).values[rightPosition]) {
                result.push_back(join((*couple1).values, "|") + "$$$" + join((*couple2).values, "|"));
            }
        }
    }
}

template<class T, class B>
vector<string> QueryExecuter<T, B>::multikeyThreeWayJoin(T *table1, T *table2, T *table3, int position1, int position1_2, int position2_3, int position3)
{
    table1->setNumberBucketFetch(0);
    table2->setNumberBucketFetch(0);
    table3->setNumberBucketFetch(0);
    table1->reset();
    table2->reset();
    table3->reset();
    cout << "Using multikeyThreeWayJoin" << endl;

    vector<string> result;
    vector<Couple> couples1;
    vector<Couple> couples2;
    vector<Couple> couples3 = table3->fetchAllCouples();
    int key_size = min(table1->keysRepartition[position1], table2->keysRepartition[position1_2]);
    for (size_t key_hash = 0; key_hash < (int) pow(2.0, (double) key_size); key_hash++) {
        couples1 = table1->fetchCouples(key_hash, key_size, position1);
        couples2 = table2->fetchCouples(key_hash, key_size, position1_2);
//        couples2 = table2->fetchCouples(key_2_3_hash, key_2_3_size, position2_3);
//        couples3 = table3->fetchCouples(key_2_3_hash, key_2_3_size, position3);

//        binaryJoinCouples(couples1, couples2, position1, position1_2, result);
//        binaryJoinCouples(couples2, couples3, position2_3, position3, result);
        threeWayJoinCouples(couples1, couples2, couples3, position1, position1_2, position2_3, position3, result);
    }

    cout << result.size() << " values successfully joined" << endl;
    cout << "table " << table1->getName() << " : " << table1->getNumberBucketFetch() << " bucket fetch" << endl;
    cout << "table " << table2->getName() << " : " << table2->getNumberBucketFetch() << " bucket fetch" << endl;
    cout << "table " << table3->getName() << " : " << table3->getNumberBucketFetch() << " bucket fetch" << endl;
    return result;
}

template<class T, class B>
void QueryExecuter<T, B>::threeWayJoinCouples(vector<Couple> &couples1, vector<Couple> &couples2, vector<Couple> &couples3, int position1, int position1_2, int position2_3, int position3, vector<string> &result)
{
    vector<Couple>::iterator couple1;
    vector<Couple>::iterator couple2;
    vector<Couple>::iterator couple3;
    for (couple1 = couples1.begin(); couple1 != couples1.end(); ++couple1) {
        for (couple2 = couples2.begin(); couple2 != couples2.end(); ++couple2) {
            if ((*couple1).values[position1] == (*couple2).values[position1_2]) {
                for (couple3 = couples3.begin(); couple3 != couples3.end(); ++couple3) {
                    if ((*couple2).values[position2_3] == (*couple3).values[position3]) {
                        result.push_back(join((*couple1).values, "|") + "$$$" + join((*couple2).values, "|") +  "$$$" + join((*couple3).values, "|"));
                    }
                }
            }
        }
    }
}

#endif // QUERYEXECUTER_H
