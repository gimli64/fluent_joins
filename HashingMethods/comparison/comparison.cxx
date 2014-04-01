#include "comparer.h"
#include <time.h>

int main()
{
    try {
        Comparer comparer = Comparer();
        BucketFactory<DepthBucket> *depthFactory = BucketFactory<DepthBucket>::getInstance();
        BucketFactory<ChainedBucket> *chainedFactory = BucketFactory<ChainedBucket>::getInstance();

        connection C("dbname=tpch user=gimli hostaddr=127.0.0.1");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        } else {
            cout << "Can't open database" << endl;
            return 1;
        }

        nontransaction N(C);
        result R( N.exec( "SELECT * FROM CUSTOMER" ));

        clock_t tStart = clock();

        cout << "\n### Extendible Hashing ###" << endl;
        ExtendibleHashing ext_hasher = ExtendibleHashing();
        for (pqxx::result::const_iterator row = R.begin(); row != R.end(); ++row) {
            ext_hasher.put(Couple(row[0].c_str(), row));
        }
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        cout << "Finished building table, serializing" << endl;
        depthFactory->setBucketNamePrefix("extendible/bucket");
        depthFactory->writeAll(ext_hasher.getBuckets());
        ext_hasher.clearBuckets();

        //    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        //        try {
        //            ext_hasher.get(*it);
        //        } catch (string &e) {
        //            cout << e << endl;
        //        }
        //    }


        cout << "\n\n### Hybrid Hashing ###" << endl;
        tStart = clock();
        HybridHashing hyb_hasher = HybridHashing();
        for (pqxx::result::const_iterator row = R.begin(); row != R.end(); ++row) {
            hyb_hasher.put(Couple(row[0].c_str(), row));
        }
        cout << "Finished building table, serializing" << endl;
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        depthFactory->setBucketNamePrefix("hybrid/bucket");
        depthFactory->writeAll(hyb_hasher.getBuckets());

        //    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        //        try {
        //            hyb_hasher.get(*it);
        //        } catch (string &e) {
        //            cout << e << endl;
        //        }
        //    }


        cout << "\n\n### Linear Hashing ###" << endl;
        tStart = clock();
        LinearHashing lin_hasher = LinearHashing();
        for (pqxx::result::const_iterator row = R.begin(); row != R.end(); ++row) {
            lin_hasher.put(Couple(row[0].c_str(), row));
        }
        cout << "Finished building table, serializing" << endl;
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        chainedFactory->setBucketNamePrefix("linear/bucket");
        chainedFactory->writeAll(lin_hasher.getBuckets());

        //    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        //        try {
        //            lin_hasher.get(*it);
        //        } catch (string &e) {
        //            cout << e << endl;
        //        }
        //    }


        cout << "Operation done successfully" << endl;
        C.disconnect ();
    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
