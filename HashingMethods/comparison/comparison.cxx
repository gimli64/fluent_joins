#include "comparer.h"
#include <time.h>

int main()
{
    try {
        Comparer comparer = Comparer();
        BucketFactory<DepthBucket> *depthFactory = BucketFactory<DepthBucket>::getInstance();
        BucketFactory<ChainedBucket> *chainedFactory = BucketFactory<ChainedBucket>::getInstance();
        clock_t tStart;

        connection C("dbname=tpch user=gimli hostaddr=127.0.0.1");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        } else {
            cout << "Can't open database" << endl;
            return 1;
        }

        nontransaction N(C);
        result R( N.exec( "SELECT * FROM CUSTOMER" ));

        cout << "\n### Extendible Hashing ###" << endl;
        tStart = clock();
        ExtendibleHashing ext_hasher = ExtendibleHashing();
        for (pqxx::result::const_iterator row = R.begin(); row != R.end(); ++row) {
            ext_hasher.put(Couple(row[0].c_str(), row));
        }
        cout << "Finished building table" << endl;
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        cout << "serializing" << endl;
        depthFactory->setBucketNamePrefix("extendible/");
        depthFactory->writeAll(ext_hasher.getBuckets());
        ext_hasher.clearBuckets();

        cout << "Getting all values" << endl;
        tStart = clock();
        for (pqxx::result::const_iterator row = R.begin(); row != R.end(); ++row) {
            try {
                ext_hasher.get(row[0].c_str());
            } catch (string &e) {
                cout << e << endl;
            }
        }
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        depthFactory->removeAll();


        cout << "\n\n### Hybrid Hashing ###" << endl;
        tStart = clock();
        HybridHashing hyb_hasher = HybridHashing();
        for (pqxx::result::const_iterator row = R.begin(); row != R.end(); ++row) {
            hyb_hasher.put(Couple(row[0].c_str(), row));
        }
        cout << "Finished building table" << endl;
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        cout << "serializing" << endl;
        depthFactory->setBucketNamePrefix("hybrid/");
        depthFactory->writeAll(hyb_hasher.getBuckets());
        hyb_hasher.clearBuckets();

        cout << "Getting all values" << endl;
        tStart = clock();
        for (pqxx::result::const_iterator row = R.begin(); row != R.end(); ++row) {
            try {
                hyb_hasher.get(row[0].c_str());
            } catch (string &e) {
                cout << e << endl;
            }
        }
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        depthFactory->removeAll();



        cout << "\n\n### Linear Hashing ###" << endl;
        tStart = clock();
        LinearHashing lin_hasher = LinearHashing();
        for (pqxx::result::const_iterator row = R.begin(); row != R.end(); ++row) {
            lin_hasher.put(Couple(row[0].c_str(), row));
        }
        cout << "Finished building table" << endl;
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        cout << "serializing" << endl;
        chainedFactory->setBucketNamePrefix("linear/");
        chainedFactory->writeAll(lin_hasher.getBuckets());
        lin_hasher.clearBuckets();

        cout << "Getting all values" << endl;
        tStart = clock();
        for (pqxx::result::const_iterator row = R.begin(); row != R.end(); ++row) {
            try {
                lin_hasher.get(row[0].c_str());
            } catch (string &e) {
                cout << e << endl;
            }
        }
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        chainedFactory->removeAll();


        cout << "Operation done successfully" << endl;
        C.disconnect ();
    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
