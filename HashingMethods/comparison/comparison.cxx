#include "tableFactory.h"
#include "queryExecuter.h"
#include <time.h>

int main()
{
    try {
        TableFactory<ExtendibleHashing, DepthBucket> factory;
        QueryExecuter<ExtendibleHashing, DepthBucket> executer;
        clock_t tStart;

        connection C("dbname=tpch user=gimli hostaddr=127.0.0.1");
        if (C.is_open()) {
            cout << "\nOpened database successfully: " << C.dbname() << endl;
        } else {
            cout << "\nCan't open database" << endl;
            return 1;
        }

        nontransaction N(C);
        result R;
        vector<int> keysRepartition;

        R = result( N.exec("SELECT * from partsupp limit 50000"));
        keysRepartition.clear();
        keysRepartition.push_back(1);
        keysRepartition.push_back(1);
        keysRepartition.push_back(0);
        keysRepartition.push_back(0);
        keysRepartition.push_back(0);
        factory.createTable(R, "partsupp", keysRepartition);

        R = result( N.exec( "SELECT * FROM supplier2 limit 10000" ));
        keysRepartition.clear();
        keysRepartition.push_back(1);
        keysRepartition.push_back(0);
        keysRepartition.push_back(0);
        keysRepartition.push_back(1);
        keysRepartition.push_back(0);
        keysRepartition.push_back(0);
        keysRepartition.push_back(0);
        factory.createTable(R, "supplier", keysRepartition);

        R = result( N.exec( "SELECT * FROM nation" ));
        keysRepartition.clear();
        keysRepartition.push_back(1);
        keysRepartition.push_back(0);
        keysRepartition.push_back(0);
        keysRepartition.push_back(0);
        factory.createTable(R, "nation", keysRepartition);

//        ExtendibleHashing *supplierTable = factory.readTable("supplier");
//        ExtendibleHashing *nationTable = factory.readTable("nation");
//        ExtendibleHashing *partsuppTable = factory.readTable("partsupp");

//        cout << "\nExecuting : select supplier.*, nation.n_name from supplier join nation on supplier.s_nationkey = nation.n_nationkey" << endl;
//        tStart = clock();
//        executer.multikeyBinaryJoin(supplierTable, nationTable, 3, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        tStart = clock();
//        executer.sortMergeBinaryJoin("supplier", 10000, "nation", 25, 3, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        cout << " \nExecuting : select partsupp.*, supplier.s_name from partsupp join supplier on partsupp.ps_suppkey = supplier.ps_suppkey" << endl;
//        tStart = clock();
//        executer.multikeyBinaryJoin(partsuppTable, supplierTable, 1, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        tStart = clock();
//        executer.sortMergeBinaryJoin("partsupp", 50000, "supplier", 10000, 1, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        cout << " \nExecuting : select partsupp.*, supplier.s_name, nation.n_name from partsupp join supplier on partsupp.ps_suppkey = supplier.ps_suppkey join nation on supplier.s_nationkey = nation.n_nationkey" << endl;
//        tStart = clock();
//        executer.multikeyThreeWayJoin(partsuppTable, supplierTable, nationTable, 1, 0, 3, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//        tStart = clock();
//        executer.sortMergeThreeWayJoin("partsupp", 50000, "supplier", 10000, "nation", 25, 1, 0, 3, 0);
//        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

        BucketFactory<DepthBucket>::getInstance()->removeAll("supplier");
        BucketFactory<DepthBucket>::getInstance()->removeAll("nation");
        BucketFactory<DepthBucket>::getInstance()->removeAll("partsupp");

        cout << "\nOperation done successfully" << endl;
        C.disconnect ();

    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
