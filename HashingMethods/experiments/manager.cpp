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

void Manager::multikeyBinaryJoin(HashTable *table1, HashTable *table2, int leftPosition, int rightPosition, int BHFmoved)
{
    int maxNumPages = 0;
    set<Bucket*> buckets1;
    set<Bucket*> buckets2;
    vector<string> result;
    int keyHashSize = min(table1->BHFsRepartitions[leftPosition], table2->BHFsRepartitions[rightPosition]);
    int sumBuckets1 = 0;
    int sumBuckets2 = 0;
    cout << "\nUsing multikeyBinaryJoin, key hash size : " << keyHashSize << endl;

    for (size_t keyHash = 0; keyHash < (int) pow(2.0, (double) keyHashSize); keyHash++) {
        buckets1 = table1->getBuckets(keyHash, keyHashSize, leftPosition);
        buckets2 = table2->getBuckets(keyHash, keyHashSize, rightPosition);
        cout << "table " << table1->name << " number pages : " << buckets1.size() << endl;
        cout << "table " << table2->name << " number pages : " << buckets2.size() << endl;
        sumBuckets1 += buckets1.size();
        sumBuckets2 += buckets2.size();
        vector<Couple> couples1;
        vector<Couple> couples2;
        extractCouples(buckets1, couples1);
        extractCouples(buckets2, couples2);
        Comparator comparator(leftPosition);
        sort(couples1.begin(), couples1.end(), comparator);
        comparator = Comparator(rightPosition);
        sort(couples2.begin(), couples2.end(), comparator);
        mergeCouples(couples1, couples2, leftPosition, rightPosition, result);
        int numPages = buckets1.size() + buckets2.size();
        if (numPages > maxNumPages)
            maxNumPages = numPages;
    }

    cout << result.size() << " tuples successfully joined." << endl;
    cout << sumBuckets1 << " buckets fetched for table " << table1->name << endl;
    cout << sumBuckets2 << " buckets fetched for table " << table2->name << endl;
//    cout << "dimension " << rightPosition << " max number pages " << maxNumPages << endl;
    numPages[rightPosition] = maxNumPages;
}

int Manager::performAllJoins()
{
//    cout << " \nJoining lineorder and customer" << endl;
    multikeyBinaryJoin(customerTable, lineorderTable, 0, 2);

//    cout << " \nJoining lineorder and part" << endl;
    multikeyBinaryJoin(partTable, lineorderTable, 0, 3);

//    cout << " \nJoining lineorder and supplier" << endl;
//    multikeyBinaryJoin(supplierTable, lineorderTable, 0, 4);

//    cout << " \nJoining lineorder and date" << endl;
//    multikeyBinaryJoin(dateTable, lineorderTable, 0, 5);

//    cout << " \nJoining lineorder and date" << endl;
//    multikeyBinaryJoin(dateTable, lineorderTable, 0, 15);

    double averageNumPages = 0.0;
    for (int i = 0; i < numPages.size(); i++) {
        averageNumPages += numPages[i];
    }
    averageNumPages /= 5;

//    cout << "Average num pages : " << averageNumPages << endl;
    return max_element(numPages.begin(), numPages.end()) - numPages.begin();
}

void Manager::extractCouples(set<Bucket*> &buckets, vector<Couple> &couples)
{
    for(set<Bucket*>::iterator bucket = buckets.begin(); bucket != buckets.end(); ++bucket)
        couples.insert(couples.end(), (*bucket)->elements.begin(), (*bucket)->elements.end());
}

void Manager::mergeCouples(vector<Couple> &couples1, vector<Couple> &couples2, int leftPosition, int rightPosition, vector<string> &result)
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
