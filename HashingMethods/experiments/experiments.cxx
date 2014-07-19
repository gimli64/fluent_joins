#include "page/bucketFactory.h"
#include "hashing/extendibleHashing.h"
#include "manager.h"

#include <time.h>
#include <pqxx/pqxx>

#include <iostream>
#include <cstdlib>

using namespace std;
using namespace pqxx;


void createTable(ExtendibleHashing& table, result relation)
{
    cout << "Building table " << table.name << endl;
    BucketFactory<Bucket>::getInstance()->reset();
    clock_t tStart = clock();

    for (int i = 0; i < relation.size(); i++) {
        table.put(Couple(relation[i]));

        if (i % 100000 == 0 && i > 0) {
            cout << "Inserted " << i << " values, time taken :  " <<  (double)(clock() - tStart)/CLOCKS_PER_SEC << "s" << endl;
        }
    }
    cout << "\nFinished building table " << table.name << endl;
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    table.printState();
}

int main()
{
    time_t start,end;
    try {
        connection C("dbname=tpch-normal user=gimli hostaddr=127.0.0.1");
        if (C.is_open()) {
            cout << "\nOpened database successfully: " << C.dbname() << endl;
        } else {
            cout << "\nCan't open database" << endl;
        }
        nontransaction N(C);
        result R;
        vector<int> BHFsRepartition;
        vector<int> interleaveOrder;

        R = result( N.exec("SELECT * from customer"));
        BHFsRepartition.clear();
        BHFsRepartition.push_back(1);
        ExtendibleHashing customerTable("customer", BHFsRepartition);
        createTable(customerTable, R);
        Manager::getInstance()->customerTable = &customerTable;

        R = result( N.exec("SELECT * from part"));
        BHFsRepartition.clear();
        BHFsRepartition.push_back(1);
        ExtendibleHashing partTable("part", BHFsRepartition);
        createTable(partTable, R);
        Manager::getInstance()->partTable = &partTable;

        R = result( N.exec("SELECT * from supplier"));
        BHFsRepartition.clear();
        BHFsRepartition.push_back(1);
        ExtendibleHashing supplierTable("supplier", BHFsRepartition);
        createTable(supplierTable, R);
        Manager::getInstance()->supplierTable = &supplierTable;

        R = result( N.exec("SELECT * from date"));
        BHFsRepartition.clear();
        BHFsRepartition.push_back(1);
        ExtendibleHashing dateTable("date", BHFsRepartition);
        createTable(dateTable, R);
        Manager::getInstance()->dateTable = &dateTable;

        R = result( N.exec("SELECT * from lineorder"));
        BHFsRepartition.clear();
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(4);
        BHFsRepartition.push_back(4);
        BHFsRepartition.push_back(4);
        BHFsRepartition.push_back(4);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(3);
        interleaveOrder.clear();
        interleaveOrder.push_back(2);
        interleaveOrder.push_back(3);
        interleaveOrder.push_back(4);
        interleaveOrder.push_back(5);
        interleaveOrder.push_back(15);
        interleaveOrder.push_back(2);
        interleaveOrder.push_back(3);
        interleaveOrder.push_back(4);
        interleaveOrder.push_back(5);
        interleaveOrder.push_back(15);
        interleaveOrder.push_back(2);
        interleaveOrder.push_back(3);
        interleaveOrder.push_back(4);
        interleaveOrder.push_back(5);
        interleaveOrder.push_back(15);
        interleaveOrder.push_back(2);
        interleaveOrder.push_back(3);
        interleaveOrder.push_back(4);
        interleaveOrder.push_back(5);
        ExtendibleHashing lineorderTable("lineorder", BHFsRepartition, interleaveOrder);
        Manager::getInstance()->lineorderTable = &lineorderTable;
        createTable(lineorderTable, R);

        Manager::getInstance()->performAllJoins();


        C.disconnect ();
    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
    }

    return 0;
}
