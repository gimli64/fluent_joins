#include "comparer.h"
#include <time.h>

int main()
{
    try {
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

        vector<int> keysRepartition;
        keysRepartition.push_back(4);
        keysRepartition.push_back(2);
        keysRepartition.push_back(2);
        keysRepartition.push_back(2);
        keysRepartition.push_back(2);
        keysRepartition.push_back(2);
        keysRepartition.push_back(2);
        keysRepartition.push_back(2);

        vector<int> sizes;
        sizes.push_back(20);
//        sizes.push_back(10000);
//        sizes.push_back(20000);
//        sizes.push_back(30000);
//        sizes.push_back(40000);
//        sizes.push_back(50000);
//        sizes.push_back(150000);

//        cout << "\nComparing three dynamic hash tables algorithms" << endl;

        for(int i = 0; i < sizes.size(); i++) {
            cout << "\n\nBucket size : " << Bucket::BUCKET_SIZE << endl;
            cout << "Input size : " << sizes[i] << endl;

            cout << "\n### Extendible Hashing ###" << endl;
            tStart = clock();
            ExtendibleHashing ext_hasher = ExtendibleHashing("extendible", keysRepartition);
            for (int j = 0; j < sizes[i]; j++) {
                ext_hasher.insert(Couple(R[j][0].c_str(), R[j]));
            }

            vector<size_t> hashes = ext_hasher.getHashes("AUTOMOBILE", 6);
            cout << hashes[0] << endl;
            cout << *(ext_hasher.getBucket(hashes[0])) << endl;
            cout << *(ext_hasher.getBucket(hashes[1])) << endl;
            cout << *(ext_hasher.getBucket(hashes[2])) << endl;
            cout << *(ext_hasher.getBucket(hashes[3])) << endl;
            cout << *(ext_hasher.getBucket(hashes[4])) << endl;
            cout << *(ext_hasher.getBucket(hashes[5])) << endl;
            cout << *(ext_hasher.getBucket(hashes[6])) << endl;
            cout << *(ext_hasher.getBucket(hashes[7])) << endl;
            cout << *(ext_hasher.getBucket(hashes[8])) << endl;
            cout << *(ext_hasher.getBucket(hashes[9])) << endl;

            cout << "Finished building table" << endl;
            printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//            cout << "serializing" << endl;
//            depthFactory->writeAll(ext_hasher.getBuckets(), ext_hasher.getBucketPath());
//            ext_hasher.clearBuckets();
//            comparer.writeTable(&ext_hasher);
//            depthFactory->reset();

//            ExtendibleHashing *new_ext_hasher = comparer.readTable("extendible");

//            cout << "Getting all values" << endl;
//            tStart = clock();
//            for (int j = 0; j < sizes[i]; j++) {
//                try {
//                    new_ext_hasher->getValue(R[j][0].c_str());
//                } catch (string &e) {
//                    cout << e << endl;
//                }
//            }
//            printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

//            cout << "\n### Hybrid Hashing ###" << endl;
//            tStart = clock();

//            HybridHashing hyb_hasher = HybridHashing("hybrid", keysRepartition);
//            for (int j = 0; j < sizes[i]; j++) {
//                hyb_hasher.insert(Couple(R[j][0].c_str(), R[j]));
//            }
//            cout << "Finished building table" << endl;
//            printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
//            cout << "serializing" << endl;
//            depthFactory->writeAll(hyb_hasher.getBuckets(), hyb_hasher.getBucketPath());
//            hyb_hasher.clearBuckets();
//            comparer.writeTable(&hyb_hasher);
//            depthFactory->removeAll("hybrid");

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
//            LinearHashing lin_hasher = LinearHashing("linear", keysRepartition);
//            for (int j = 0; j < sizes[i]; j++) {
//                lin_hasher.insert(Couple(R[j][0].c_str(), R[j]));
//            }
//            cout << "Finished building table" << endl;
//            printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
//            cout << "serializing" << endl;
//            chainedFactory->writeAll(lin_hasher.getBuckets(), lin_hasher.getBucketPath());
//            lin_hasher.clearBuckets();
//            comparer.writeTable(&lin_hasher);
//            chainedFactory->reset();

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
