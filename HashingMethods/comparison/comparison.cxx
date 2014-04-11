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
            cout << "Opened database successfully: " << C.dbname() << endl;
        } else {
            cout << "Can't open database" << endl;
            return 1;
        }

        cout << "\nCreating the tables" << endl;
        tStart = clock();
        nontransaction N(C);
        result R( N.exec( "SELECT * FROM supplier" ));
        comparer.createTable(R, "supplier");

        R = result( N.exec( "SELECT * FROM nation" ));
        comparer.createTable(R, "nation");
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

        cout << "\nExecuting : select supplier.*, nation.n_name from supplier join nation on supplier.s_nationkey = nation.n_nationkey" << endl;
        HybridHashing *supplierTable = comparer.readTable("supplier");
        HybridHashing *nationTable = comparer.readTable("nation");
//        comparer.binaryJoin(supplierTable, nationTable);

        cout << "\nOperation done successfully" << endl;
        C.disconnect ();
    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
