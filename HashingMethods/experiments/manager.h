#ifndef MANAGER_H
#define MANAGER_H

#include <vector>
#include <set>

#include "hashing/hashTable.h"
#include "page/bucket.h"

using namespace std;

class Manager
{
public:
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
    void multikeyBinaryJoin(HashTable *table1, HashTable *table2, int leftPosition, int rightPosition);
};

#endif // MANAGER_H
