#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <set>
#include <boost/algorithm/string/join.hpp>

#include "hashing/hashTable.h"
#include "page/bucket.h"

using namespace std;

class Manager
{
public:
    static const int PAGE_SIZE = 4096;

    HashTable *customerTable;
    HashTable *partTable;
    HashTable *supplierTable;
    HashTable *dateTable;
    HashTable *lineorderTable;

    vector<int> numPages;

    static Manager* getInstance();

    int performAllJoins();

private:
    static Manager* instance;

    Manager();
    void multikeyBinaryJoin(HashTable *table1, HashTable *table2, int leftPosition, int rightPosition, int BHFmoved=0);

    void extractCouples(set<Bucket *> &buckets, vector<Couple> &couples);
    void mergeCouples(vector<Couple> &couples1, vector<Couple> &couples2, int leftPosition, int rightPosition, vector<string> &result);
};

#endif // MANAGER_H
