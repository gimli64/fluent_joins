#include "manager.h"

Manager* Manager::instance = 0;

Manager::Manager()
{
    Tseek = 9.;
    Ttransfer = 0.04;
    Tsort = 0.24;
    Tmerge = 0.004;
    Tblacklist = 0.01;
}

Manager* Manager::getInstance()
{
    if (instance == 0)
        instance = new Manager();
    return instance;
}

double Manager::costRead(int numberPages, int commonBHFs)
{
    return pow(2.0, commonBHFs) * Tseek + numberPages * (Ttransfer + Tblacklist);
}

double Manager::costWrite(int numberPages, int pageBuffers)
{
    return ceil((double) numberPages / pageBuffers) * Tseek +  numberPages * Ttransfer;
}

double Manager::costSortMerge(int numberPages)
{
    return numberPages * (Tsort + Tmerge);
}

void Manager::multikeyBinaryJoin(HashTable *table1, HashTable *table2, int leftPosition, int rightPosition)
{
    vector<string> result;
    double cost;

    int numberBuckets1 = table1->getNumberBuckets();
    int numberBuckets2 = table2->getNumberBuckets();

    int commonBHFs = min(table1->BHFsRepartitions[leftPosition], table2->BHFsRepartitions[rightPosition]);
    cout << "\nUsing multikeyBinaryJoin, common BHFs : " << commonBHFs << endl;

    vector<size_t> setHashes1(table1->BHFsRepartitions.size(), 0);
    vector<size_t> setHashes2(table2->BHFsRepartitions.size(), 0);
    vector<int> hashSizes1(table1->BHFsRepartitions.size(), 0);
    hashSizes1[leftPosition] = commonBHFs;
    vector<int> hashSizes2(table2->BHFsRepartitions.size(), 0);
    hashSizes2[rightPosition] = commonBHFs;

    int remainBHFs1 = table1->getNumberBHFs() - commonBHFs;
    int remainBHFs2 = table2->getNumberBHFs() - commonBHFs;
    int allocatedPages1 = (int) pow(2.0, (double) remainBHFs1);
    int allocatedPages2 = (int) pow(2.0, (double) remainBHFs2);
    int allocatedPagesResult = NUM_PAGES - allocatedPages1 - allocatedPages2;

    int numberBHFsMoved = 0;
    if (allocatedPagesResult <= 0) {
        cout << "Not enough pages in memory, adapting execution" << endl;
        vector<int> BHFs(table2->BHFsRepartitions.begin(), table2->BHFsRepartitions.end());
        while (allocatedPagesResult <= 0) {
            int i = 0;
            while(i < BHFs.size() && (BHFs[i] == 0 || i == rightPosition)) i++;
            if (i == BHFs.size()) {
                cout << "Cannot move any more BHFs : Semi fluent join !" << endl;
                return;
            }
            cout << "Moving BHFs from position " << i << endl;
            while(allocatedPagesResult <= 0 && BHFs[i] > 0) {
                BHFs[i] -= 1;
                hashSizes2[i] += 1;
                allocatedPages2 /= 2;
                allocatedPagesResult = NUM_PAGES - allocatedPages1 - allocatedPages2;
            }
            cout << "Moved " << hashSizes2[i] << " BHFs" << endl;
        }
        numberBHFsMoved = accumulate(hashSizes2.begin(), hashSizes2.end(), 0) - hashSizes2[rightPosition];
    }

    cout << "Allocated " << allocatedPages1 << " memory pages for " << table1->name << endl;
    cout << "Allocated " << allocatedPages2 << " memory pages for " << table2->name << endl;
    cout << "Allocated " << allocatedPagesResult << " memory pages for result" << endl;

    cost += costRead(numberBuckets1, commonBHFs);
    cout << "read table " << table1->name << " cost : " << costRead(numberBuckets1, commonBHFs) << endl;
    cost += costRead(numberBuckets2, commonBHFs + numberBHFsMoved);
    cout << "read table " << table2->name << " cost : " << costRead(numberBuckets2, commonBHFs + numberBHFsMoved) << endl;
    cost += costWrite((double) table2->numberItems / Bucket::BUCKET_SIZE, allocatedPagesResult);
    cout << "write cost : " << costWrite((double) table2->numberItems / Bucket::BUCKET_SIZE, allocatedPagesResult) << endl;
    cost += costSortMerge(numberBuckets1 + numberBuckets2);
    cout << "sort merge cost : " << costSortMerge(numberBuckets1 + numberBuckets2) << endl;

    clock_t tStart = clock();
    for (size_t setHash = 0; setHash < (int) pow(2.0, (double) hashSizes1[leftPosition]); setHash++) {
        setHashes1[leftPosition] = setHash;
        setHashes2[rightPosition] = setHash;
        set<Bucket*> buckets1;
        set<Bucket*> buckets2;
        table1->getBuckets(setHashes1, hashSizes1, buckets1);
        cout << "table " << table1->name << " number pages : " << buckets1.size() << endl;
        vector<Couple> couples1;
        extractCouples(buckets1, couples1);
        Comparator comparator(leftPosition);
        sort(couples1.begin(), couples1.end(), comparator);

        if (numberBHFsMoved > 0) {
            for(size_t movedHash = 0; movedHash < (int) pow(2.0, (double) numberBHFsMoved); movedHash++) {
                size_t hashValue = movedHash;
                for(int i = 0; i < hashSizes2.size(); i++) {
                    if (i != rightPosition) {
                        setHashes2[i] = hashValue & ((1 << hashSizes2[i]) - 1);
                        hashValue >>= hashSizes2[i];
                    }
                }
                int size = buckets2.size();
                table2->getBuckets(setHashes2, hashSizes2, buckets2);
                cout << "table " << table2->name << " number pages : " << (buckets2.size() - size) << endl;
            }
        } else {
            table2->getBuckets(setHashes2, hashSizes2, buckets2);
            cout << "table " << table2->name << " number pages : " << buckets2.size() << endl;
        }

        vector<Couple> couples2;
        extractCouples(buckets2, couples2);
        comparator = Comparator(rightPosition);
        sort(couples2.begin(), couples2.end(), comparator);
        mergeCouples(couples1, couples2, leftPosition, rightPosition, result);
    }
    printf("Time taken: %.7fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    cout << result.size() << " tuples successfully joined." << endl;
    cout << "cost : " << cost << endl;
}

int Manager::performAllJoins()
{
    //    cout << " \nJoining lineorder and customer" << endl;
//    multikeyBinaryJoin(customerTable, lineorderTable, 0, 2);

    //    cout << " \nJoining lineorder and part" << endl;
    //    multikeyBinaryJoin(partTable, lineorderTable, 0, 3);

    //    cout << " \nJoining lineorder and supplier" << endl;
    multikeyBinaryJoin(supplierTable, lineorderTable, 0, 4);

    //    cout << "Average num pages : " << averageNumPages << endl;
    //    return max_element(numPages.begin(), numPages.end()) - numPages.begin();
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
