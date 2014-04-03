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
        result R( N.exec( "SELECT * FROM PART" ));

        cout << "\nComparing three dynamic hash tables algorithms" << endl;
        cout << "Bucket size : " << Bucket::BUCKET_SIZE << endl;

        cout << "\n### Extendible Hashing ###" << endl;
        tStart = clock();
        ExtendibleHashing ext_hasher = ExtendibleHashing();
        ext_hasher.setBucketPath("extendible/");
        for (pqxx::result::const_iterator row = R.begin(); row != R.end(); ++row) {
            ext_hasher.put(Couple(row[0].c_str(), row));
        }
        cout << "Finished building table" << endl;
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        cout << "serializing" << endl;
        depthFactory->writeAll(ext_hasher.getBuckets(), ext_hasher.getBucketPath());
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
        depthFactory->reset();

        cout << "\n\n### Hybrid Hashing ###" << endl;
        tStart = clock();
        HybridHashing hyb_hasher = HybridHashing();
        hyb_hasher.setBucketPath("hybrid/");
        for (pqxx::result::const_iterator row = R.begin(); row != R.end(); ++row) {
            hyb_hasher.put(Couple(row[0].c_str(), row));
        }
        cout << "Finished building table" << endl;
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        cout << "serializing" << endl;
        depthFactory->writeAll(hyb_hasher.getBuckets(), hyb_hasher.getBucketPath());
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
        depthFactory->reset();



        cout << "\n\n### Linear Hashing ###" << endl;
        tStart = clock();
        LinearHashing lin_hasher = LinearHashing();
        lin_hasher.setBucketPath("linear/");
        for (pqxx::result::const_iterator row = R.begin(); row != R.end(); ++row) {
            lin_hasher.put(Couple(row[0].c_str(), row));
        }
        cout << "Finished building table" << endl;
        printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        cout << "serializing" << endl;
        chainedFactory->writeAll(lin_hasher.getBuckets(), lin_hasher.getBucketPath());
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
        chainedFactory->reset();


        cout << "Operation done successfully" << endl;
        C.disconnect ();
    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
