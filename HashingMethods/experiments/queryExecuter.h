#ifndef QUERYEXECUTER_H
#define QUERYEXECUTER_H

#include "hashing/extendibleHashing.h"
#include "page/bucketFactory.h"

#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/lexical_cast.hpp>
#include <time.h>
#include <boost/thread.hpp>

using namespace boost;

template<class T, class B> class QueryExecuter
{
public:

    QueryExecuter();
    vector<Couple> readStandardTable(string fileName);

    vector<string> sortMergeBinaryJoin(string name1, string name2, int leftPosition, int rightPosition);
    static void mergeCouples(vector<Couple> &couples1, vector<Couple> &couples2, int leftPosition, int rightPosition, vector<string> &result);

    vector<string> multikeyBinaryJoin(T* table1, T* table2, int leftPosition, int rightPosition);
    vector<string> multikeyBinaryJoinThreaded(T* table1, T* table2, int leftPosition, int rightPosition);
    static void * binaryJoinCouples(T *table1, T *table2, int leftPosition, int rightPosition, size_t keyHash, int keyHashSize);
};

template<class T, class B>
QueryExecuter<T, B>::QueryExecuter()
{
}

template<class T, class B>
vector<Couple> QueryExecuter<T, B>::readStandardTable(string fileName)
{
    vector<Couple> couples;
    string line;
    vector<string> values;
    ifstream file (fileName.c_str());

    if (file.is_open()) {
        while (getline(file,line)) {
            split(values, line, is_any_of("|"));
            couples.push_back(Couple(values[0], values));
        }
        file.close();
    }

    else cout << "Unable to open file" << endl;

    return couples;
}

template<class T, class B>
vector<string> QueryExecuter<T, B>::sortMergeBinaryJoin(string name1, string name2, int leftPosition, int rightPosition)
{
    cout << "Using sort merge join" << endl;

    vector<string> result;

    string tableName1 = "/tmp/tables/" + name1 + "_aux";
    string tableName2 = "/tmp/tables/" + name2 + "_aux";
    std::system(("exec sort -t '|' -k " + lexical_cast<string>(leftPosition + 1) + " " + tableName1 + " > " + tableName1 + "_sorted").c_str());
    std::system(("exec sort -t '|' -k " + lexical_cast<string>(rightPosition + 1) + " " + tableName2 + " > " + tableName2 + "_sorted").c_str());

    vector<Couple> couples1 = readStandardTable(tableName1 + "_sorted");
    vector<Couple> couples2 = readStandardTable(tableName2 + "_sorted");
    mergeCouples(couples1, couples2, leftPosition, rightPosition, result);
    cout << result.size() << " values successfully joined" << endl;
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

    for (size_t keyHash = 0; keyHash < (int) pow(2.0, (double) keyHashSize); keyHash++) {
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
vector<string> QueryExecuter<T, B>::multikeyBinaryJoinThreaded(T *table1, T *table2, int leftPosition, int rightPosition)
{
    table1->setNumberBucketFetch(0);
    table1->reset();
    table2->setNumberBucketFetch(0);
    table2->reset();

    vector<string> result;
    vector<thread *> workerThreads;
    int keyHashSize = min(table1->keysRepartition[leftPosition], table2->keysRepartition[rightPosition]);
    cout << "Using multikeyBinaryJoin threaded, key hash size : " << keyHashSize << endl;

    int numberThreads = (int) pow(2.0, (double) keyHashSize);

    for (size_t keyHash = 0; keyHash < numberThreads; keyHash++) {
        workerThreads.push_back(new thread(binaryJoinCouples, table1, table2, leftPosition, rightPosition, keyHash, keyHashSize));
    }
    for (size_t i = 0; i < numberThreads; i++) {
        workerThreads[i]->join();
        delete workerThreads[i];
    }
    cout << "Values successfully joined" << endl;

    //    cout << result.size() << " values successfully joined" << endl;
    //    cout << "table " << table1->getName() << " : " << table1->getNumberBucketFetch() << " bucket fetch" << endl;
    //    cout << "table " << table2->getName() << " : " << table2->getNumberBucketFetch() << " bucket fetch" << endl;
    return result;
}

template<class T, class B>
void * QueryExecuter<T, B>::binaryJoinCouples(T *table1, T *table2, int leftPosition, int rightPosition, size_t keyHash, int keyHashSize)
{
    vector<string> result;
    vector<Couple> couples1;
    vector<Couple> couples2;

    couples1 = table1->fetchCouples(keyHash, keyHashSize, leftPosition);
    couples2 = table2->fetchCouples(keyHash, keyHashSize, rightPosition);
    Comparator comparator(leftPosition);
    sort(couples1.begin(), couples1.end(), comparator);
    comparator = Comparator(rightPosition);
    sort(couples2.begin(), couples2.end(), comparator);
    mergeCouples(couples1, couples2, leftPosition, rightPosition, result);
    //    cout << result.size() << " values joined from thread " << keyHash << endl;
    //    return result;
}

#endif // QUERYEXECUTER_H
