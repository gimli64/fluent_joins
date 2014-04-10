#include "comparer.h"
#include <time.h>

int main()
{
    try {
        Comparer<ExtendibleHashing> comparer;
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
        vector<int> sizes;
        sizes.push_back(5000);
//        sizes.push_back(10000);
//        sizes.push_back(20000);
//        sizes.push_back(30000);
//        sizes.push_back(40000);
//        sizes.push_back(50000);
//        sizes.push_back(60000);

//        cout << "\nComparing three dynamic hash tables algorithms" << endl;

        for(int i = 0; i < sizes.size(); i++) {
            cout << "\n\nBucket size : " << Bucket::BUCKET_SIZE << endl;
            cout << "Input size : " << sizes[i] << endl;

            cout << "\n### Extendible Hashing ###" << endl;
            tStart = clock();
            ExtendibleHashing ext_hasher = ExtendibleHashing("extendible");
            for (int j = 0; j < sizes[i]; j++) {
                ext_hasher.put(Couple(R[j][0].c_str(), R[j]));
            }
            cout << "Finished building table" << endl;
            printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
            cout << "serializing" << endl;
            depthFactory->writeAll(ext_hasher.getBuckets(), ext_hasher.getBucketPath());
            ext_hasher.clearBuckets();
            comparer.writeTable(&ext_hasher);

            ExtendibleHashing *new_ext_hasher = comparer.readTable("extendible");

            cout << "Getting all values" << endl;
            tStart = clock();
            for (int j = 0; j < sizes[i]; j++) {
                try {
                    new_ext_hasher->get(R[j][0].c_str());
                } catch (string &e) {
                    cout << e << endl;
                }
            }
            printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

            cout << "\n### Hybrid Hashing ###" << endl;
            tStart = clock();
//            HybridHashing hyb_hasher = HybridHashing("hybrid");
//            for (int j = 0; j < sizes[i]; j++) {
//                hyb_hasher.put(Couple(R[j][0].c_str(), R[j]));
//            }
//            cout << "Finished building table" << endl;
//            printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
//            cout << "serializing" << endl;
//            depthFactory->writeAll(hyb_hasher.getBuckets(), hyb_hasher.getBucketPath());
//            hyb_hasher.clearBuckets();
//            comparer.writeTable(&hyb_hasher);

//            HybridHashing *new_hybrid_hasher = comparer.readTable("hybrid");

//            cout << "Getting all values" << endl;
//            tStart = clock();
//            for (int j = 0; j < sizes[i]; j++) {
//                try {
//                    new_hybrid_hasher->get(R[j][0].c_str());
//                } catch (string &e) {
//                    cout << e << endl;
//                }
//            }
//            printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);


//            cout << "\n### Linear Hashing ###" << endl;
//            tStart = clock();
//            LinearHashing lin_hasher = LinearHashing("linear");
//            for (int j = 0; j < sizes[i]; j++) {
//                lin_hasher.put(Couple(R[j][0].c_str(), R[j]));
//            }
//            cout << "Finished building table" << endl;
//            printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
//            cout << "serializing" << endl;
//            chainedFactory->writeAll(lin_hasher.getBuckets(), lin_hasher.getBucketPath());
//            lin_hasher.clearBuckets();
//            comparer.writeTable(&lin_hasher);

//            LinearHashing *new_lin_hasher = comparer.readTable("linear");
//            cout << "Getting all values" << endl;
//            tStart = clock();
//            for (int j = 0; j < sizes[i]; j++) {
//                try {
//                    new_lin_hasher->get(R[j][0].c_str());
//                } catch (string &e) {
//                    cout << e << endl;
//                }
//            }
//            printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        }


        cout << "Operation done successfully" << endl;
        C.disconnect ();
    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
