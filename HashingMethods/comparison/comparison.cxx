#include "comparer.h"
#include <time.h>

int main()
{
    try {
        Comparer<HybridHashing, DepthBucket> comparer;
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

        cout << "\nCreating the tables\n" << endl;
        tStart = clock();
        nontransaction N(C);
        result R( N.exec( "SELECT * FROM supplier limit 50" ));
        vector<int> keysRepartition;
        keysRepartition.push_back(2);
        keysRepartition.push_back(0);
        keysRepartition.push_back(0);
        keysRepartition.push_back(1);
        keysRepartition.push_back(0);
        keysRepartition.push_back(0);
        keysRepartition.push_back(0);

        comparer.createTable(R, "supplier", keysRepartition);

        R = result( N.exec( "SELECT * FROM nation" ));
        keysRepartition.clear();
        keysRepartition.push_back(1);
        keysRepartition.push_back(1);
        keysRepartition.push_back(1);
        keysRepartition.push_back(0);
        comparer.createTable(R, "nation", keysRepartition);

        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

        cout << "\nExecuting : select supplier.*, nation.n_name from supplier join nation on supplier.s_nationkey = nation.n_nationkey" << endl;
        tStart = clock();
        HybridHashing *supplierTable = comparer.readTable("supplier");
        HybridHashing *nationTable = comparer.readTable("nation");
        comparer.binaryJoin(supplierTable, nationTable, 3, 0);
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        cout << "table supplier : " << supplierTable->getNumberBucketFetch() << " bucket fetch" << endl;
        cout << "table nation :" << nationTable->getNumberBucketFetch() << " bucket fetch" << endl;

        cout << "\nOperation done successfully" << endl;
        C.disconnect ();
    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
