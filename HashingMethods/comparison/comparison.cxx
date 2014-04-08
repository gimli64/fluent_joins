#include "comparer.h"
#include <time.h>

int main()
{
    try {
        Comparer<HybridHashing, DepthBucket> comparer;
        clock_t tStart;

        connection C("dbname=tpch user=gimli hostaddr=127.0.0.1");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        } else {
            cout << "Can't open database" << endl;
            return 1;
        }

        nontransaction N(C);
        result R( N.exec( "SELECT * FROM customer" ));

        cout << "Bucket size : " << Bucket::BUCKET_SIZE << endl;

        tStart = clock();
        comparer.createTable(R, "customer", 90);
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

        HybridHashing *hasher = comparer.readTable("customer");

//        cout << "Getting all values" << endl;
//        tStart = clock();
//        for (int j = 0; j < R.size(); j++) {
//            try {
//                hasher->get(R[j][0].c_str());
//            } catch (string &e) {
//                cout << e << endl;
//            }
//        }
//        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

        cout << "Operation done successfully" << endl;
        C.disconnect ();
    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
