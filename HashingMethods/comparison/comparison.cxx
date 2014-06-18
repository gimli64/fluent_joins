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

int main()
{
    try {
        TableFactory<ExtendibleHashing, DepthBucket> factory;
        QueryExecuter<ExtendibleHashing, DepthBucket> executer;
        clock_t tStart;

        connection C("dbname=tpch-normal user=gimli hostaddr=127.0.0.1");
        if (C.is_open()) {
            cout << "\nOpened database successfully: " << C.dbname() << endl;
        } else {
            cout << "\nCan't open database" << endl;
            return 1;
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

//        R = result( N.exec("SELECT * from lineorder limit 1000000"));
//        BHFsRepartition.clear();
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(5);
//        BHFsRepartition.push_back(5);
//        BHFsRepartition.push_back(5);
//        interleaveOrder.clear();
//        interleaveOrder.push_back(2);
//        interleaveOrder.push_back(3);
//        interleaveOrder.push_back(4);
//        interleaveOrder.push_back(2);
//        interleaveOrder.push_back(3);
//        interleaveOrder.push_back(4);
//        interleaveOrder.push_back(2);
//        interleaveOrder.push_back(3);
//        interleaveOrder.push_back(4);
//        interleaveOrder.push_back(2);
//        interleaveOrder.push_back(3);
//        interleaveOrder.push_back(4);
//        interleaveOrder.push_back(2);
//        interleaveOrder.push_back(3);
//        interleaveOrder.push_back(4);
//        factory.createTable(R, "lineorder", BHFsRepartition, interleaveOrder);

//        R = result( N.exec("SELECT * from lineorder limit 1000000"));
//        BHFsRepartition.clear();
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(0);
//        BHFsRepartition.push_back(1);
//        BHFsRepartition.push_back(1);
//        BHFsRepartition.push_back(1);
//        factory.createAutomatedTable(R, "lineorder2", BHFsRepartition);

        ExtendibleHashing *customerTable = factory.readTable("customer");
        ExtendibleHashing *partTable = factory.readTable("part");
        ExtendibleHashing *supplierTable = factory.readTable("supplier");
        ExtendibleHashing *dateTable = factory.readTable("date");
        ExtendibleHashing *lineorderTable = factory.readTable("lineorder");

        /***** sort merge *****/
//        cout << " \nJoining lineorder and customer" << endl;
//        tStart = clock();
//        executer.sortMergeBinaryJoin(lineorderTable, customerTable, 2, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        cout << " \nJoining lineorder and part" << endl;
//        tStart = clock();
//        executer.sortMergeBinaryJoin(lineorderTable, partTable, 3, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        cout << " \nJoining lineorder and supplier" << endl;
//        tStart = clock();
//        executer.sortMergeBinaryJoin(lineorderTable, supplierTable, 4, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        cout << " \nJoining lineorder, customer and part" << endl;
//        tStart = clock();
//        executer.sortMergeThreeWayJoin(partTable, lineorderTable, customerTable, 0, 3, 2, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

        /***** Standard table ******/
        cout << " \nJoining lineorder and customer" << endl;
        tStart = clock();
        executer.multikeyBinaryJoin(lineorderTable, customerTable, 2, 0);
        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        cout << " \nJoining lineorder and part" << endl;
//        tStart = clock();
//        executer.multikeyBinaryJoin(lineorderTable, partTable, 3, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        cout << " \nJoining lineorder and supplier" << endl;
//        tStart = clock();
//        executer.multikeyBinaryJoin(lineorderTable, supplierTable, 4, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

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
//        lineorderTable = factory.readTable("lineorder2");

//        cout << " \nJoining lineorder and customer" << endl;
//        tStart = clock();
//        executer.multikeyBinaryJoin(lineorderTable, customerTable, 2, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        cout << " \nJoining lineorder and part" << endl;
//        tStart = clock();
//        executer.multikeyBinaryJoin(lineorderTable, partTable, 3, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        cout << " \nJoining lineorder and supplier" << endl;
//        tStart = clock();
//        executer.multikeyBinaryJoin(lineorderTable, supplierTable, 4, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

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

        cout << "\nOperation done successfully" << endl;
        C.disconnect ();
    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
