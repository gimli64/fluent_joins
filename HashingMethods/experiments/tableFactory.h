#ifndef TABLEFACTORY_H
#define TABLEFACTORY_H

#include "hashing/extendibleHashing.h"
#include "page/bucketFactory.h"

#include <vector>
#include <fstream>
#include <sys/resource.h>
#include <time.h>
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <boost/algorithm/string/join.hpp>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;
using namespace boost;

template<class T, class B> class TableFactory
{
public:
    TableFactory();

    void createTable(result relation, string name, vector<int> keysRepartition, vector<int> interleaveOrder = vector<int>());

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
    system("exec mkdir -p /tmp/tables");
    system(("exec mkdir -p /tmp/buckets/" + name).c_str());
    BucketFactory<B>::getInstance()->reset();
    T table(name, keysRepartition, interleaveOrder);
    clock_t tStart = clock();

    bool automated = (interleaveOrder.size() == 0);
    int totalNumberKeys = 0;
    for (int i = 0; i < keysRepartition.size(); i++)
        totalNumberKeys += keysRepartition[i];
    insertionLimit = (int) pow(2.0, totalNumberKeys) * B::BUCKET_SIZE;

    //    srand (unsigned(std::time(0)));
    //    random_shuffle(relation.begin(), relation.end());

    for (int i = 0; i < relation.size(); i++) {
        table.putMultikey(Couple(relation[i][0].c_str(), relation[i]));

        if (automated && i >= insertionLimit) {
            if(table.addBHF()) {
                insertionLimit *= 2;
            } else {
                insertionLimit *= 1.1;
            }
        }

        if (i % 100000 == 0 && i > 0) {
            cout << "Inserted " << i << " values, time taken :  " <<  (double)(clock() - tStart)/CLOCKS_PER_SEC << "s" << endl;
//            table.printState();
//            table.dimensionStats(2);
//            table.dimensionStats(3);
//            table.dimensionStats(4);
//            table.dimensionStats(5);
//            cout << "\n" << endl;
        }
    }
    cout << "\n\nFinished building table " << name << endl;
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
//    table.dimensionStats(2);
    table.printState();

//    cout << "Writing table " << name << " to disk\n" << endl;
//    writeTable(&table);

//    cout << "Building standard table\n" << endl;
//    ofstream standardTableFile (("/tmp/tables/" + name + "_aux").c_str());
//    if (standardTableFile.is_open()) {
//        for (int i = 0; i < relation.size(); i++) {
//            for (int j = 0; j < relation[i].size(); j++) {
//                standardTableFile << relation[i][j].c_str() << "|";
//            }
//            standardTableFile << "\n";
//        }
//        standardTableFile.close();
//    } else cout << "Unable to open file";
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
