#include "tableFactory.h"
#include "queryExecuter.h"
#include <time.h>

#include <iostream>
#include <cstdlib>

using namespace std;

//#define NUM_THREADS     5

//void workerFunc()
//{
//    posix_time::seconds workTime(3);
//    cout << "Worker running" << endl;
//    this_thread::sleep(workTime);
//    cout << "Worker finished" << endl;
//}

//int main ()
//{
//    cout << "main startup" << endl;
//    thread workerThread(workerFunc);
//    cout << "waiting for thread" << endl;
//    workerThread.join();
//    cout << "main done" << endl;
//    return 0;
//}

void createTables(TableFactory<ExtendibleHashing, DepthBucket> factory)
{
    try {
        connection C("dbname=tpch user=gimli hostaddr=127.0.0.1");
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
        //        factory.createAutomatedTable(R, "customer", BHFsRepartition);

//        R = result( N.exec("SELECT * from part"));
//        BHFsRepartition.clear();
//        BHFsRepartition.push_back(1);
//        factory.createAutomatedTable(R, "part", BHFsRepartition);

//        R = result( N.exec("SELECT * from supplier"));
//        BHFsRepartition.clear();
//        BHFsRepartition.push_back(1);
//        factory.createAutomatedTable(R, "supplier", BHFsRepartition);

        //        R = result( N.exec("SELECT * from date"));
        //        BHFsRepartition.clear();
        //        BHFsRepartition.push_back(1);
        //        factory.createAutomatedTable(R, "date", BHFsRepartition);

        R = result( N.exec("SELECT * from partsupp limit 1000000"));
        BHFsRepartition.clear();
        BHFsRepartition.push_back(8);
        BHFsRepartition.push_back(7);
        interleaveOrder.clear();
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(1);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(1);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(1);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(1);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(1);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(1);
        interleaveOrder.push_back(0);
        interleaveOrder.push_back(1);
        interleaveOrder.push_back(0);
        factory.createTable(R, "partsupp", BHFsRepartition, interleaveOrder);

//        R = result( N.exec("SELECT * from partsupp limit 1000000"));
//        BHFsRepartition.clear();
//        BHFsRepartition.push_back(1);
//        BHFsRepartition.push_back(1);
//        factory.createAutomatedTable(R, "partsupp2", BHFsRepartition);

        C.disconnect ();
    } catch (const std::exception &e) {
        cerr << e.what() << std::endl;
    }
}

int main()
{
    TableFactory<ExtendibleHashing, DepthBucket> factory;
    QueryExecuter<ExtendibleHashing, DepthBucket> executer;
    time_t start,end;

    createTables(factory);

//    ExtendibleHashing *customerTable = factory.readTable("customer");
    ExtendibleHashing *partTable = factory.readTable("part");
    ExtendibleHashing *supplierTable = factory.readTable("supplier");
    ExtendibleHashing *partsuppTable = factory.readTable("partsupp");
//    ExtendibleHashing *dateTable = factory.readTable("date");
//    ExtendibleHashing *lineorderTable = factory.readTable("lineorder");

    /***** sort merge *****/
//    cout << " \nJoining lineorder and customer" << endl;
//    time(&start);
//    executer.sortMergeBinaryJoin(lineorderTable, customerTable, 2, 0);
//    time (&end);
//    printf("Time taken: %.2fs\n\n", difftime (end,start));

    cout << " \nJoining lineorder and part" << endl;
    time(&start);
    executer.sortMergeBinaryJoin(partsuppTable, partTable, 0, 0);
    time (&end);
    printf("Time taken: %.2fs\n\n", difftime (end,start));

    cout << " \nJoining lineorder and supplier" << endl;
    time(&start);
    executer.sortMergeBinaryJoin(partsuppTable, supplierTable, 1, 0);
    time (&end);
    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //        cout << " \nJoining lineorder, customer and part" << endl;
    //        tStart = clock();
    //        executer.sortMergeThreeWayJoin(partTable, lineorderTable, customerTable, 0, 3, 2, 0);
    //        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    /***** Standard table ******/
//    cout << " \nJoining lineorder and customer" << endl;
//    time (&start);
//    executer.multikeyBinaryJoin(lineorderTable, customerTable, 2, 0);
//    time (&end);
//    printf("Time taken: %.2fs\n\n", difftime (end,start));

    cout << " \nJoining lineorder and part" << endl;
    time (&start);
    executer.multikeyBinaryJoin(partsuppTable, partTable, 0, 0);
    time (&end);
    printf("Time taken: %.2fs\n\n", difftime (end,start));

    cout << " \nJoining lineorder and supplier" << endl;
    time (&start);
    executer.multikeyBinaryJoin(partsuppTable, supplierTable, 1, 0);
    time (&end);
    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //        cout << " \nJoining lineorder and date" << endl;
    //        tStart = clock();
    //        executer.multikeyBinaryJoin(lineorderTable, dateTable, 5, 0);
    //        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    //        cout << " \nJoining lineorder and date" << endl;
    //        tStart = clock();
    //        executer.multikeyBinaryJoin(lineorderTable, dateTable, 15, 0);
    //        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    //        cout << " \nJoining lineorder, customer and part" << endl;
    //        tStart = clock();
    //        executer.multikeyThreeWayJoin(partTable, lineorderTable, customerTable, 0, 3, 2, 0);
    //        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    /***** Automated scheme table ******/
    partsuppTable = factory.readTable("partsupp2");

//    cout << " \nJoining lineorder and customer" << endl;
//    time (&start);
//    executer.multikeyBinaryJoin(lineorderTable, customerTable, 2, 0);
//    time (&end);
//    printf("Time taken: %.2fs\n\n", difftime (end,start));

    cout << " \nJoining lineorder and part" << endl;
    time (&start);
    executer.multikeyBinaryJoin(partsuppTable, partTable, 0, 0);
    time (&end);
    printf("Time taken: %.2fs\n\n", difftime (end,start));

    cout << " \nJoining lineorder and supplier" << endl;
    time (&start);
    executer.multikeyBinaryJoin(partsuppTable, supplierTable, 1, 0);
    time (&end);
    printf("Time taken: %.2fs\n\n", difftime (end,start));

    //        cout << " \nJoining lineorder and date" << endl;
    //        tStart = clock();
    //        executer.multikeyBinaryJoin(lineorderTable, dateTable, 5, 0);
    //        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    //        cout << " \nJoining lineorder and date" << endl;
    //        tStart = clock();
    //        executer.multikeyBinaryJoin(lineorderTable, dateTable, 15, 0);
    //        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    //        cout << " \nJoining lineorder, customer and part" << endl;
    //        tStart = clock();
    //        executer.multikeyThreeWayJoin(partTable, lineorderTable, customerTable, 0, 3, 2, 0);
    //        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    return 0;
}
