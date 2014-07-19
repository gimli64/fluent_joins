#include "manager.h"

Manager* Manager::instance = 0;

Manager::Manager()
{
    numPages = vector<int>(16, 0);
}

Manager* Manager::getInstance()
{
    if (instance == 0)
        instance = new Manager();
    return instance;
}

void Manager::multikeyBinaryJoin(HashTable *table1, HashTable *table2, int leftPosition, int rightPosition)
{
    int maxNumPages = 0;
    set<Bucket*> buckets1;
    set<Bucket*> buckets2;
    int keyHashSize = min(table1->BHFsRepartitions[leftPosition], table2->BHFsRepartitions[rightPosition]);
//    cout << "Using multikeyBinaryJoin, key hash size : " << keyHashSize << endl;

    for (size_t keyHash = 0; keyHash < (int) pow(2.0, (double) keyHashSize); keyHash++) {
        buckets1 = table1->getBuckets(keyHash, keyHashSize, leftPosition);
        buckets2 = table2->getBuckets(keyHash, keyHashSize, rightPosition);
//        cout << "table " << table1->name << " number pages : " << buckets1.size() << endl;
//        cout << "table " << table2->name << " number pages : " << buckets2.size() << endl;
        int numPages = buckets1.size() + buckets2.size();
        if (numPages > maxNumPages)
            maxNumPages = numPages;
    }

    cout << "dimension " << leftPosition << " max number pages " << maxNumPages << endl;
    numPages[leftPosition] = maxNumPages;
}

int Manager::performAllJoins()
{
//    cout << " \nJoining lineorder and customer" << endl;
    multikeyBinaryJoin(lineorderTable, customerTable, 2, 0);

//    cout << " \nJoining lineorder and part" << endl;
    multikeyBinaryJoin(lineorderTable, partTable, 3, 0);

//    cout << " \nJoining lineorder and supplier" << endl;
    multikeyBinaryJoin(lineorderTable, supplierTable, 4, 0);

//    cout << " \nJoining lineorder and date" << endl;
    multikeyBinaryJoin(lineorderTable, dateTable, 5, 0);

//    cout << " \nJoining lineorder and date" << endl;
    multikeyBinaryJoin(lineorderTable, dateTable, 15, 0);

    double averageNumPages = 0.0;
    for (int i = 0; i < numPages.size(); i++) {
        averageNumPages += numPages[i];
    }
    averageNumPages /= 5;

    cout << "Average num pages : " << averageNumPages << endl;
    return max_element(numPages.begin(), numPages.end()) - numPages.begin();
}
