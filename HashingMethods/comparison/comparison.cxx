#include "tableFactory.h"
#include "queryExecuter.h"
#include <time.h>

int main()
{
    try {
        TableFactory<ExtendibleHashing, DepthBucket> factory;
        QueryExecuter<ExtendibleHashing, DepthBucket> executer;
        clock_t tStart;

        connection C("dbname=movielens user=gimli hostaddr=127.0.0.1");
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

        //        R = result( N.exec("SELECT * from part"));
        //        BHFsRepartition.clear();
        //        BHFsRepartition.push_back(12);
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
        //        interleaveOrder.push_back(0);
        //        interleaveOrder.push_back(0);
        //        factory.createTable(R, "part", BHFsRepartition, interleaveOrder);

        R = result( N.exec("SELECT * from ratings limit 20000"));
        BHFsRepartition.clear();
        BHFsRepartition.push_back(1);
        BHFsRepartition.push_back(1);
        BHFsRepartition.push_back(1);
        factory.createAutomatedTable(R, "ratings", BHFsRepartition);

//        R = result( N.exec("SELECT * from partsupp"));
//        BHFsRepartition.clear();
//        BHFsRepartition.push_back(8);
//        BHFsRepartition.push_back(7);
//        interleaveOrder.clear();
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(1);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(1);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(1);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(1);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(1);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(1);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(1);
//        interleaveOrder.push_back(0);
//        factory.createTable(R, "partsupp2", BHFsRepartition, interleaveOrder);

//        R = result( N.exec( "SELECT * FROM supplier" ));
//        BHFsRepartition.clear();
//        BHFsRepartition.push_back(7);
//        interleaveOrder.clear();
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        interleaveOrder.push_back(0);
//        factory.createTable(R, "supplier", BHFsRepartition, interleaveOrder);

//        ExtendibleHashing *supplierTable = factory.readTable("supplier");
//        ExtendibleHashing *partsuppTable = factory.readTable("partsupp");
//        ExtendibleHashing *partTable = factory.readTable("part");

//        cout << " \nExecuting : select partsupp.*, supplier.s_name from partsupp join supplier on partsupp.ps_suppkey = supplier.ps_suppkey" << endl;
//        tStart = clock();
//        executer.multikeyBinaryJoin(partsuppTable, supplierTable, 1, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        cout << " \nExecuting : select partsupp.*, part.p_name from partsupp join part on partsupp.ps_partkey = part.p_partkey" << endl;
//        tStart = clock();
//        executer.multikeyBinaryJoin(partTable, partsuppTable, 0, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        cout << " \nExecuting : select partsupp.*, supplier.s_name, part.p_name from partsupp join supplier on partsupp.ps_suppkey = supplier.ps_suppkey join part on partsupp.ps_partkey = part.p_partkey" << endl;
//        tStart = clock();
//        executer.multikeyThreeWayJoin(partTable, partsuppTable, supplierTable, 0, 0, 1, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);


//        partsuppTable = factory.readTable("partsupp2");
//        cout << " \nExecuting : select partsupp.*, supplier.s_name from partsupp join supplier on partsupp.ps_suppkey = supplier.ps_suppkey" << endl;
//        tStart = clock();
//        executer.multikeyBinaryJoin(partsuppTable, supplierTable, 1, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        cout << " \nExecuting : select partsupp.*, part.p_name from partsupp join part on partsupp.ps_partkey = part.p_partkey" << endl;
//        tStart = clock();
//        executer.multikeyBinaryJoin(partTable, partsuppTable, 0, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        cout << " \nExecuting : select partsupp.*, supplier.s_name, part.p_name from partsupp join supplier on partsupp.ps_suppkey = supplier.ps_suppkey join part on partsupp.ps_partkey = part.p_partkey" << endl;
//        tStart = clock();
//        executer.multikeyThreeWayJoin(partTable, partsuppTable, supplierTable, 0, 0, 1, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);


        //        BucketFactory<DepthBucket>::getInstance()->removeAll("partsupp");
        //        BucketFactory<DepthBucket>::getInstance()->removeAll("part");
        //        BucketFactory<DepthBucket>::getInstance()->removeAll("supplier");

        cout << "\nOperation done successfully" << endl;
        C.disconnect ();

    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
