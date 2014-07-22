#include "page/bucketFactory.h"
#include "hashing/extendibleHashing.h"
#include "manager.h"

#include <time.h>
#include <pqxx/pqxx>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/join.hpp>

#include <iostream>
#include <cstdlib>

using namespace std;
using namespace boost;
using namespace pqxx;


void createTable(ExtendibleHashing& table, result relation)
{
    cout << "Building table " << table.name << endl;
    clock_t tStart = clock();

    for (int i = 0; i < relation.size(); i++) {
        Couple couple(relation[i]);
        couple.key = i;
        table.put(couple);

        if (i % 100000 == 0 && i > 0) {
            cout << "Inserted " << i << " values, time taken :  " <<  (double)(clock() - tStart)/CLOCKS_PER_SEC << "s" << endl;
        }
    }
    cout << "\nFinished building table " << table.name << endl;
    printf("Time taken: %.7fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    table.printState();
}

int main()
{
    time_t start,end;
    try {
        connection C("dbname=tpch-skewed-4 user=gimli hostaddr=127.0.0.1");
        if (C.is_open()) {
            cout << "\nOpened database successfully: " << C.dbname() << endl;
        } else {
            cout << "\nCan't open database" << endl;
        }
        nontransaction N(C);
        result R;
        vector<int> BHFsRepartition;
        vector<int> interleaveOrder;

        //        R = result( N.exec("SELECT * from customer"));
        //        BHFsRepartition.clear();
        //        BHFsRepartition.push_back(1);
        //        BucketFactory<Bucket>::getInstance()->reset();
        //        ExtendibleHashing customerTable("customer", BHFsRepartition);
        //        createTable(customerTable, R);
        //        Manager::getInstance()->customerTable = &customerTable;

        //        R = result( N.exec("SELECT * from part"));
        //        BHFsRepartition.clear();
        //        BHFsRepartition.push_back(1);
        //        BucketFactory<Bucket>::getInstance()->reset();
        //        ExtendibleHashing partTable("part", BHFsRepartition);
        //        createTable(partTable, R);
        //        Manager::getInstance()->partTable = &partTable;

        //        R = result( N.exec("SELECT * from supplier"));
        //        BHFsRepartition.clear();
        //        BHFsRepartition.push_back(1);
        //        BucketFactory<Bucket>::getInstance()->reset();
        //        ExtendibleHashing supplierTable("supplier", BHFsRepartition);
        //        createTable(supplierTable, R);
        //        Manager::getInstance()->supplierTable = &supplierTable;

        //        R = result( N.exec("SELECT * from date"));
        //        BHFsRepartition.clear();
        //        BHFsRepartition.push_back(1);
        //        BucketFactory<Bucket>::getInstance()->reset();
        //        ExtendibleHashing dateTable("date", BHFsRepartition);
        //        createTable(dateTable, R);
        //        Manager::getInstance()->dateTable = &dateTable;

        R = result( N.exec("SELECT * from lineorder"));
        BHFsRepartition.clear();
        BHFsRepartition.push_back(7);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(7);
        BHFsRepartition.push_back(7);
        BHFsRepartition.push_back(7);
        interleaveOrder.clear();
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(2);
        interleaveOrder.push_back(3);
        interleaveOrder.push_back(4);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(2);
        interleaveOrder.push_back(3);
        interleaveOrder.push_back(4);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(2);
        interleaveOrder.push_back(3);
        interleaveOrder.push_back(4);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(2);
        interleaveOrder.push_back(3);
        interleaveOrder.push_back(4);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(2);
        interleaveOrder.push_back(3);
        interleaveOrder.push_back(4);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(2);
        interleaveOrder.push_back(3);
        interleaveOrder.push_back(4);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(2);
        interleaveOrder.push_back(3);
        interleaveOrder.push_back(4);
        BucketFactory<Bucket>::getInstance()->reset();
        ExtendibleHashing lineorderTable("lineorder", BHFsRepartition, interleaveOrder);
        Manager::getInstance()->lineorderTable = &lineorderTable;
        createTable(lineorderTable, R);

//        Manager::getInstance()->performAllJoins();


        C.disconnect ();
    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
    }

    return 0;
}
