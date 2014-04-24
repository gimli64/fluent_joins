#include "comparer.h"
#include <time.h>

int main()
{
    try {
        Comparer<MultikeyHybridHashing, DepthBucket> comparer;
        clock_t tStart;

        cout << "Fluent joins experiment" << endl;
        cout << "Bucket size : " << Bucket::BUCKET_SIZE << endl;

        connection C("dbname=tpch user=gimli hostaddr=127.0.0.1");
        if (C.is_open()) {
            cout << "\nOpened database successfully: " << C.dbname() << endl;
        } else {
            cout << "\nCan't open database" << endl;
            return 1;
        }

        cout << "\nCreating the tables" << endl;
        nontransaction N(C);
        result R;
        vector<int> keysRepartition;

        R = result( N.exec("SELECT * from partsupp limit 9000"));
        keysRepartition.clear();
        keysRepartition.push_back(6);
        keysRepartition.push_back(5);
        keysRepartition.push_back(0);
        keysRepartition.push_back(0);
        keysRepartition.push_back(0);
        tStart = clock();
        comparer.createTable(R, "partsupp", keysRepartition);
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

        R = result( N.exec( "SELECT * FROM supplier" ));
        keysRepartition.clear();
        keysRepartition.push_back(5);
        keysRepartition.push_back(1);
        keysRepartition.push_back(1);
        keysRepartition.push_back(2);
        keysRepartition.push_back(1);
        keysRepartition.push_back(1);
        keysRepartition.push_back(0);
        tStart = clock();
        comparer.createTable(R, "supplier", keysRepartition);
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

        R = result( N.exec( "SELECT * FROM nation" ));
        keysRepartition.clear();
        keysRepartition.push_back(2);
        keysRepartition.push_back(0);
        keysRepartition.push_back(0);
        keysRepartition.push_back(0);
        tStart = clock();
        comparer.createTable(R, "nation", keysRepartition);
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

        cout << "\nExecuting : select supplier.*, nation.n_name from supplier join nation on supplier.s_nationkey = nation.n_nationkey" << endl;
        MultikeyHybridHashing *supplierTable = comparer.readTable("supplier");
        MultikeyHybridHashing *nationTable = comparer.readTable("nation");

        tStart = clock();
        comparer.multikeyBinaryJoin(supplierTable, nationTable, 3, 0);
        cout << "table supplier : " << supplierTable->getNumberBucketFetch() << " bucket fetch" << endl;
        cout << "table nation : " << nationTable->getNumberBucketFetch() << " bucket fetch" << endl;
        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

        tStart = clock();
        comparer.sortMergeBinaryJoin(supplierTable, nationTable, 3, 0);
        cout << "table supplier : " << supplierTable->getNumberBucketFetch() << " bucket fetch" << endl;
        cout << "table nation : " << nationTable->getNumberBucketFetch() << " bucket fetch" << endl;
        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

        cout << " \nExecuting : select partsupp.*, supplier.s_name, nation.n_name from partsupp join supplier on partsupp.ps_suppkey = supplier.ps_suppkey join nation on supplier.s_nationkey = nation.n_nationkey" << endl;
        MultikeyHybridHashing *partsuppTable = comparer.readTable("partsupp");

        tStart = clock();
        comparer.multikeyThreeWayJoin(partsuppTable, supplierTable, nationTable, 1, 0, 3, 0);
        cout << "table partsupp : " << partsuppTable->getNumberBucketFetch() << " bucket fetch" << endl;
        cout << "table supplier : " << supplierTable->getNumberBucketFetch() << " bucket fetch" << endl;
        cout << "table nation : " << nationTable->getNumberBucketFetch() << " bucket fetch" << endl;
        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

        tStart = clock();
        comparer.sortMergeThreeWayJoin(partsuppTable, supplierTable, nationTable, 1, 0, 3, 0);
        cout << "table partsupp : " << partsuppTable->getNumberBucketFetch() << " bucket fetch" << endl;
        cout << "table supplier : " << supplierTable->getNumberBucketFetch() << " bucket fetch" << endl;
        cout << "table nation : " << nationTable->getNumberBucketFetch() << " bucket fetch" << endl;
        printf("Time taken: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

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
