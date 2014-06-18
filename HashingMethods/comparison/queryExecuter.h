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

    vector<string> result;
    vector<Couple> couples1;
    vector<Couple> couples2;
    int keyHashSize = min(table1->keysRepartition[leftPosition], table2->keysRepartition[rightPosition]);
    cout << "Using multikeyBinaryJoin, key hash size : " << keyHashSize << endl;

//    #pragma omp parallel for
    for (size_t keyHash = 0; keyHash < (int) pow(2.0, (double) keyHashSize); keyHash++) {
//        table1->reset();
//        table2->reset();
        couples1 = table1->fetchCouples(keyHash, keyHashSize, leftPosition);
        couples2 = table2->fetchCouples(keyHash, keyHashSize, rightPosition);
        Comparator comparator(leftPosition);
        sort(couples1.begin(), couples1.end(), comparator);
        comparator = Comparator(rightPosition);
        sort(couples2.begin(), couples2.end(), comparator);
        mergeCouples(couples1, couples2, leftPosition, rightPosition, result);
        cout << result.size() << " values joined." << endl;
    }

    cout << result.size() << " values successfully joined" << endl;
    cout << "table " << table1->getName() << " : " << table1->getNumberBucketFetch() << " bucket fetch" << endl;
    cout << "table " << table2->getName() << " : " << table2->getNumberBucketFetch() << " bucket fetch" << endl;
    return result;
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
    table3->loadBuckets();
    cout << "Using multikeyThreeWayJoin" << endl;

    vector<string> result;
    vector<Couple> couples1;
    vector<Couple> couples2;
    vector<Couple> couples3;
    int key_1_2_size = min(table1->keysRepartition[position1], table2->keysRepartition[position1_2]);
    int key_2_3_size = min(table2->keysRepartition[position2_3], table3->keysRepartition[position3]);
    for (size_t key_1_2_hash = 0; key_1_2_hash < (int) pow(2.0, (double) key_1_2_size); key_1_2_hash++) {
        table1->reset();
        table2->reset();
        couples1 = table1->fetchCouples(key_1_2_hash, key_1_2_size, position1);
        table2->loadBuckets(key_1_2_hash, key_1_2_size, position1_2);

        for (size_t key_2_3_hash = 0; key_2_3_hash < (int) pow(2.0, (double) key_2_3_size); key_2_3_hash++) {
            table3->reset();
            couples2 = table2->getCouples(key_1_2_hash, key_1_2_size, position1_2, key_2_3_hash, key_2_3_size, position2_3);
            couples3 = table3->fetchCouples(key_2_3_hash, key_2_3_size, position3);

            Comparator comparator(position1);
            sort(couples1.begin(), couples1.end(), comparator);
            comparator = Comparator(position1_2);
            sort(couples2.begin(), couples2.end(), comparator);
            vector<Couple> interCouples;
            threeWayMergeCouples(couples2, couples1, position1_2, position1, interCouples);
            comparator = Comparator(position2_3);
            sort(interCouples.begin(), interCouples.end(), comparator);
            comparator = Comparator(position3);
            sort(couples3.begin(), couples3.end(), comparator);
            mergeCouples(interCouples, couples3, position2_3, position3, result);
        }
        cout << result.size() << " values joined." << endl;
    }

    cout << result.size() << " values successfully joined" << endl;
    cout << "table " << table1->getName() << " : " << table1->getNumberBucketFetch() << " bucket fetch" << endl;
    cout << "table " << table2->getName() << " : " << table2->getNumberBucketFetch() << " bucket fetch" << endl;
    cout << "table " << table3->getName() << " : " << table3->getNumberBucketFetch() << " bucket fetch" << endl;
    return result;
}


#endif // QUERYEXECUTER_H
