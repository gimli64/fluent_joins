#include "tableFactory.h"
#include "queryExecuter.h"
#include <time.h>

#include <iostream>
#include <cstdlib>

using namespace std;

void createTables(TableFactory<ExtendibleHashing, DepthBucket> factory)
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

        R = result( N.exec("SELECT * from customer"));
        BHFsRepartition.clear();
        BHFsRepartition.push_back(1);
        factory.createTable(R, "customer", BHFsRepartition);

        //        R = result( N.exec("SELECT * from part"));
        //        BHFsRepartition.clear();
        //        BHFsRepartition.push_back(1);
        //        factory.createTable(R, "part", BHFsRepartition);

        //        R = result( N.exec("SELECT * from supplier"));
        //        BHFsRepartition.clear();
        //        BHFsRepartition.push_back(1);
        //        factory.createTable(R, "supplier", BHFsRepartition);

        //        R = result( N.exec("SELECT * from date"));
        //        BHFsRepartition.clear();
        //        BHFsRepartition.push_back(1);
        //        factory.createTable(R, "date", BHFsRepartition);

        //        R = result( N.exec("SELECT * from lineorder limit 1000000"));
        //        BHFsRepartition.clear();
        //        BHFsRepartition.push_back(0);
        //        BHFsRepartition.push_back(0);
        //        BHFsRepartition.push_back(5);
        //        BHFsRepartition.push_back(5);
        //        BHFsRepartition.push_back(4);
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
        //        factory.createTable(R, "lineorder", BHFsRepartition, interleaveOrder);

        //        R = result( N.exec("SELECT * from lineorder limit 1000000"));
        //        BHFsRepartition.clear();
        //        BHFsRepartition.push_back(0);
        //        BHFsRepartition.push_back(0);
        //        BHFsRepartition.push_back(1);
        //        BHFsRepartition.push_back(1);
        //        BHFsRepartition.push_back(1);
        //        factory.createTable(R, "lineorder2", BHFsRepartition);

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

    //    createTables(factory);

    ExtendibleHashing *customerTable = factory.readTable("customer");
    ExtendibleHashing *partTable = factory.readTable("part");
    ExtendibleHashing *supplierTable = factory.readTable("supplier");
    //    ExtendibleHashing *partsuppTable = factory.readTable("partsupp");
    //    ExtendibleHashing *dateTable = factory.readTable("date");
    ExtendibleHashing *lineorderTable = factory.readTable("lineorder");

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

    cout << "MULTIKEY JOINS" << endl;
    cout << " \nJoining lineorder and customer" << endl;
    time (&start);
    executer.multikeyBinaryJoin(lineorderTable, customerTable, 2, 0);
    time (&end);
    printf("Time taken: %.2fs\n\n", difftime (end,start));

    cout << " \nJoining lineorder and part" << endl;
    time (&start);
    executer.multikeyBinaryJoin(lineorderTable, partTable, 3, 0);
    time (&end);
    printf("Time taken: %.2fs\n\n", difftime (end,start));

    cout << " \nJoining lineorder and supplier" << endl;
    time (&start);
    executer.multikeyBinaryJoin(lineorderTable, supplierTable, 4, 0);
    time (&end);
    printf("Time taken: %.2fs\n\n", difftime (end,start));

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

    cout << "MULTIKEY JOINS, AUTOMATED TABLE" << endl;
    lineorderTable = factory.readTable("lineorder2");

    cout << " \nJoining lineorder and customer" << endl;
    time (&start);
    executer.multikeyBinaryJoin(lineorderTable, customerTable, 2, 0);
    time (&end);
    printf("Time taken: %.2fs\n\n", difftime (end,start));

    cout << " \nJoining lineorder and part" << endl;
    time (&start);
    executer.multikeyBinaryJoin(lineorderTable, partTable, 3, 0);
    time (&end);
    printf("Time taken: %.2fs\n\n", difftime (end,start));

    cout << " \nJoining lineorder and supplier" << endl;
    time (&start);
    executer.multikeyBinaryJoin(lineorderTable, supplierTable, 4, 0);
    time (&end);
    printf("Time taken: %.2fs\n\n", difftime (end,start));

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

    return 0;
}
