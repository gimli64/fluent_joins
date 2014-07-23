#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <set>
#include <numeric>
#include <boost/algorithm/string/join.hpp>

#include "hashing/hashTable.h"
#include "page/bucket.h"

using namespace std;

class Manager
{
public:
    static const int PAGE_SIZE = 4096;
    static const int NUM_PAGES = 1000;

    double Tseek;
    double Ttransfer;
    double Tsort;
    double Tmerge;
    double Tblacklist;

    HashTable *customerTable;
    HashTable *partTable;
    HashTable *supplierTable;
    HashTable *dateTable;
    HashTable *lineorderTable;

    static Manager* getInstance();

    int performAllJoins();

private:
    static Manager* instance;

    Manager();
    void multikeyBinaryJoin(HashTable *table1, HashTable *table2, int leftPosition, int rightPosition);

    double costRead(int numberPages, int commonBHFs);
    double costWrite(int numberPages, int pageBuffers);
    double costSortMerge(int numberPages);

    void extractCouples(set<Bucket *> &buckets, vector<Couple> &couples);
    void mergeCouples(vector<Couple> &couples1, vector<Couple> &couples2, int leftPosition, int rightPosition, vector<string> &result);
};

#endif // MANAGER_H
