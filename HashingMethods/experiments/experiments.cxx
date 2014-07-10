#include "page/bucketFactory.h"
#include "queryExecuter.h"

#include <time.h>
#include <pqxx/pqxx>

#include <iostream>
#include <cstdlib>

using namespace std;
using namespace pqxx;

void createTable(result relation, string name, vector<int> keysRepartition, vector<int> interleaveOrder)
{
    cout << "Building table " << name << endl;
    BucketFactory<Bucket>::getInstance()->reset();
    ExtendibleHashing table(name, keysRepartition, interleaveOrder);
    clock_t tStart = clock();

    bool automated = (interleaveOrder.size() == 0);
    int totalNumberKeys = 0;
    for (int i = 0; i < keysRepartition.size(); i++)
        totalNumberKeys += keysRepartition[i];
    int insertionLimit = (int) pow(2.0, totalNumberKeys) * Bucket::BUCKET_SIZE;

    srand (unsigned(std::time(0)));
    random_shuffle(relation.begin(), relation.end());

    for (int i = 0; i < relation.size(); i++) {
        Couple couple(relation[i]);
        couple.values[0] = lexical_cast<string>(i);
        table.put(Couple(relation[i]));
//        table.put(couple);

        if (automated && i >= insertionLimit) {
            if(table.addBHF()) {
                insertionLimit *= 2;
            } else {
                insertionLimit *= 1.1;
            }
        }

        if (i % 100000 == 0 && i > 0) {
            cout << "Inserted " << i << " values, time taken :  " <<  (double)(clock() - tStart)/CLOCKS_PER_SEC << "s" << endl;
        }
    }
    cout << "\n\nFinished building table " << name << endl;
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    table.printState();
}

void createTables()
{
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
        vector<int> interleaveOrder;

//        R = result( N.exec("SELECT * from customer"));
//        BHFsRepartition.clear();
//        BHFsRepartition.push_back(10);
//        interleaveOrder.clear();
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        createTable(R, "customer", BHFsRepartition, interleaveOrder);

        //        R = result( N.exec("SELECT * from part"));
        //        BHFsRepartition.clear();
        //        BHFsRepartition.push_back(1);
        //        createTable(R, "part", BHFsRepartition);

        //        R = result( N.exec("SELECT * from supplier"));
        //        BHFsRepartition.clear();
        //        BHFsRepartition.push_back(1);
        //        createTable(R, "supplier", BHFsRepartition);

        //        R = result( N.exec("SELECT * from date"));
        //        BHFsRepartition.clear();
        //        BHFsRepartition.push_back(1);
        //        createTable(R, "date", BHFsRepartition);

        R = result( N.exec("SELECT * from lineorder limit 1000000"));
        BHFsRepartition.clear();
        BHFsRepartition.push_back(5);
        BHFsRepartition.push_back(1);
        BHFsRepartition.push_back(3);
        BHFsRepartition.push_back(2);
        BHFsRepartition.push_back(2);
        BHFsRepartition.push_back(2);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(0);
        BHFsRepartition.push_back(2);
        interleaveOrder.clear();
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(1);
        interleaveOrder.push_back(2);
        interleaveOrder.push_back(3);
        interleaveOrder.push_back(4);
        interleaveOrder.push_back(5);
        interleaveOrder.push_back(15);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(2);
        interleaveOrder.push_back(3);
        interleaveOrder.push_back(4);
        interleaveOrder.push_back(5);
        interleaveOrder.push_back(15);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(2);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(0);
        createTable(R, "lineorder", BHFsRepartition, interleaveOrder);

        C.disconnect ();
    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
    }
}

int main()
{
    QueryExecuter<ExtendibleHashing, Bucket> executer;
    time_t start,end;

    createTables();

    //    ExtendibleHashing *customerTable = factory.readTable("customer");
    //    ExtendibleHashing *partTable = factory.readTable("part");
    //    ExtendibleHashing *supplierTable = factory.readTable("supplier");
    //    ExtendibleHashing *partsuppTable = factory.readTable("partsupp");
    //    ExtendibleHashing *dateTable = factory.readTable("date");
    //    ExtendibleHashing *lineorderTable = factory.readTable("lineorder");


    /********************************************
    ****************** SORT MERGE ***************
    ********************************************/
    //    cout << "SORT MERGE JOINS" << endl;
    //    cout << " \nJoining lineorder and customer" << endl;
    //    time(&start);
    //    executer.sortMergeBinaryJoin("lineorder", "customer", 2, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and part" << endl;
    //    time(&start);
    //    executer.sortMergeBinaryJoin(lineorderTable, partTable, 3, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and supplier" << endl;
    //    time(&start);
    //    executer.sortMergeBinaryJoin(lineorderTable, supplierTable, 4, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and date" << endl;
    //    time (&start);
    //    executer.sortMergeBinaryJoin(lineorderTable, dateTable, 5, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));


    /********************************************
    ****************** MULTIKEY *****************
    ********************************************/
    //    cout << "MULTIKEY JOINS" << endl;
    //    cout << " \nJoining lineorder and customer" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoin(lineorderTable, customerTable, 2, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and part" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoin(lineorderTable, partTable, 3, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and supplier" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoin(lineorderTable, supplierTable, 4, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and date" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoin(lineorderTable, dateTable, 5, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));


    /*****************************************************
    ****************** MULTIKEY THREADED *****************
    *****************************************************/
    //    cout << "MULTIKEY JOINS THREADED" << endl;
    //    cout << " \nJoining lineorder and customer" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoinThreaded(lineorderTable, customerTable, 2, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and part" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoinThreaded(lineorderTable, partTable, 3, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and supplier" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoinThreaded(lineorderTable, supplierTable, 4, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and date" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoinThreaded(lineorderTable, dateTable, 5, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));


    /****************************************************
    ****************** MULTIKEY AUTOMATED ***************
    ****************************************************/
    //    cout << "MULTIKEY JOINS, AUTOMATED TABLE" << endl;
    //    lineorderTable = factory.readTable("lineorder2");

    //    cout << " \nJoining lineorder and customer" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoin(lineorderTable, customerTable, 2, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and part" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoin(lineorderTable, partTable, 3, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and supplier" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoin(lineorderTable, supplierTable, 4, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and date" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoin(lineorderTable, dateTable, 5, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));


    /*************************************************************
    ****************** MULTIKEY AUTOMATED THREADED ***************
    *************************************************************/
    //    cout << "MULTIKEY JOINS THREADED, AUTOMATED TABLE" << endl;
    //    cout << " \nJoining lineorder and customer" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoinThreaded(lineorderTable, customerTable, 2, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and part" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoinThreaded(lineorderTable, partTable, 3, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and supplier" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoinThreaded(lineorderTable, supplierTable, 4, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //    cout << " \nJoining lineorder and date" << endl;
    //    time (&start);
    //    executer.multikeyBinaryJoinThreaded(lineorderTable, dateTable, 5, 0);
    //    time (&end);
    //    printf("Time taken: %.2fs\n\n", difftime (end,start));

    return 0;
}
