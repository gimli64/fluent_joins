#include "page/bucketFactory.h"
#include "hashing/extendibleHashing.h"

#include <time.h>
#include <pqxx/pqxx>

#include <iostream>
#include <cstdlib>

using namespace std;
using namespace pqxx;

void multikeyBinaryJoin(ExtendibleHashing &table1, ExtendibleHashing &table2, int leftPosition, int rightPosition)
{
    set<Bucket*> buckets1;
    set<Bucket*> buckets2;
    int keyHashSize = min(table1.BHFsRepartitions[leftPosition], table2.BHFsRepartitions[rightPosition]);
    cout << "Using multikeyBinaryJoin, key hash size : " << keyHashSize << endl;

    for (size_t keyHash = 0; keyHash < (int) pow(2.0, (double) keyHashSize); keyHash++) {
        buckets1 = table1.getBuckets(keyHash, keyHashSize, leftPosition);
        buckets2 = table2.getBuckets(keyHash, keyHashSize, rightPosition);
        cout << "table " << table1.name << " number pages : " << buckets1.size() << endl;
        cout << "table " << table2.name << " number pages : " << buckets2.size() << endl;
    }
}

ExtendibleHashing createTable(result relation, string name, vector<int> BHFsRepartitions)
{
    cout << "Building table " << name << endl;
    BucketFactory<Bucket>::getInstance()->reset();
    ExtendibleHashing table(name, BHFsRepartitions);
    clock_t tStart = clock();

    int totalNumberKeys = 0;
    for (int i = 0; i < BHFsRepartitions.size(); i++)
        totalNumberKeys += BHFsRepartitions[i];

//    srand (unsigned(std::time(0)));
//    random_shuffle(relation.begin(), relation.end());

    for (int i = 0; i < relation.size(); i++) {
        table.put(Couple(relation[i]));

        if (i % 100000 == 0 && i > 0) {
            cout << "Inserted " << i << " values, time taken :  " <<  (double)(clock() - tStart)/CLOCKS_PER_SEC << "s" << endl;
        }
    }
    cout << "\nFinished building table " << name << endl;
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    table.printState();
    return table;
}

void createTables()
{
}

int main()
{
    time_t start,end;

    try {
        connection C("dbname=tpch-skewed-part user=gimli hostaddr=127.0.0.1");
        if (C.is_open()) {
            cout << "\nOpened database successfully: " << C.dbname() << endl;
        } else {
            cout << "\nCan't open database" << endl;
        }
        nontransaction N(C);
        result R;
        vector<int> BHFsRepartition;

        R = result( N.exec("SELECT * from customer"));
        BHFsRepartition.clear();
        BHFsRepartition.push_back(1);
        ExtendibleHashing customerTable = createTable(R, "customer", BHFsRepartition);

        R = result( N.exec("SELECT * from part"));
        BHFsRepartition.clear();
        BHFsRepartition.push_back(1);
        ExtendibleHashing partTable = createTable(R, "part", BHFsRepartition);

        R = result( N.exec("SELECT * from supplier"));
        BHFsRepartition.clear();
        BHFsRepartition.push_back(1);
        ExtendibleHashing supplierTable = createTable(R, "supplier", BHFsRepartition);

        R = result( N.exec("SELECT * from date"));
        BHFsRepartition.clear();
        BHFsRepartition.push_back(1);
        ExtendibleHashing dateTable = createTable(R, "date", BHFsRepartition);

//        R = result( N.exec("SELECT * from lineorder limit 1000000"));
//        BHFsRepartition.clear();
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(4);
//        BHFsRepartition.push_back(3);
//        BHFsRepartition.push_back(4);
//        BHFsRepartition.push_back(3);
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(3);
//        interleaveOrder.clear();
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(2);
//        interleaveOrder.push_back(3);
//        interleaveOrder.push_back(4);
//        interleaveOrder.push_back(5);
//        interleaveOrder.push_back(15);
//        interleaveOrder.push_back(2);
//        interleaveOrder.push_back(3);
//        interleaveOrder.push_back(4);
//        interleaveOrder.push_back(5);
//        interleaveOrder.push_back(15);
//        interleaveOrder.push_back(2);
//        interleaveOrder.push_back(3);
//        interleaveOrder.push_back(4);
//        interleaveOrder.push_back(5);
//        interleaveOrder.push_back(15);
//        interleaveOrder.push_back(2);
//        interleaveOrder.push_back(4);
//        ExtendibleHashing lineorderTable = createTable(R, "lineorder", BHFsRepartition, interleaveOrder);

//        cout << " \nJoining lineorder and customer" << endl;
//        time (&start);
//        multikeyBinaryJoin(lineorderTable, customerTable, 2, 0);
//        time (&end);
//        printf("Time taken: %.2fs\n\n", difftime (end,start));

//        cout << " \nJoining lineorder and part" << endl;
//        time (&start);
//        multikeyBinaryJoin(lineorderTable, partTable, 3, 0);
//        time (&end);
//        printf("Time taken: %.2fs\n\n", difftime (end,start));

//        cout << " \nJoining lineorder and supplier" << endl;
//        time (&start);
//        multikeyBinaryJoin(lineorderTable, supplierTable, 4, 0);
//        time (&end);
//        printf("Time taken: %.2fs\n\n", difftime (end,start));

//        cout << " \nJoining lineorder and date" << endl;
//        time (&start);
//        multikeyBinaryJoin(lineorderTable, dateTable, 5, 0);
//        time (&end);
//        printf("Time taken: %.2fs\n\n", difftime (end,start));

//        cout << " \nJoining lineorder and date" << endl;
//        time (&start);
//        multikeyBinaryJoin(lineorderTable, dateTable, 15, 0);
//        time (&end);
//        printf("Time taken: %.2fs\n\n", difftime (end,start));


        C.disconnect ();
    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
    }

    return 0;
}
