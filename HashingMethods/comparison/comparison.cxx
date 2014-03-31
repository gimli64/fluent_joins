#include "comparer.h"
#include <time.h>

int main()
{
    try {
        Comparer comparer = Comparer();
        connection C("dbname=tpch user=gimli hostaddr=127.0.0.1");
        if (C.is_open()) {
            cout << "Opened database successfully: " << C.dbname() << endl;
        } else {
            cout << "Can't open database" << endl;
            return 1;
        }

        nontransaction N(C);
        result R( N.exec( "SELECT * FROM NATION" ));

        tuple t = R[0];
        cout << t["n_nationkey"].as<string>() << endl;
//        for (result::const_iterator c = R.begin(); c != R.end(); ++c) {
//           cout << "nation_key = " << c[0].as<int>() << endl;
//           cout << "Name = " << c[1].as<string>() << endl;
//           cout << "region_key = " << c[2].as<int>() << endl;
//           cout << "comment = " << c[3].as<string>() << endl;
//        }

        clock_t tStart = clock();

        cout << "Comparing three dynamic hashing algorightms" << endl;

        //    cout << "\n\n### Extendible Hashing ###" << endl;
        //    ExtendibleHashing ext_hasher = ExtendibleHashing();
        //    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        //        ext_hasher.put(Couple(*it, *it));
        //    }

        //    BucketFactory<DepthBucket>::getInstance()->writeAll(ext_hasher.getBuckets());
        //    ext_hasher.clearBuckets();

        //    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        //        try {
        //            ext_hasher.get(*it);
        //        } catch (string &e) {
        //            cout << e << endl;
        //        }
        //    }
        //    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        //    BucketFactory<DepthBucket>::getInstance()->removeAll();


        //    cout << "\n\n### Hybrid Hashing ###" << endl;
        //    tStart = clock();
        //    HybridHashing hyb_hasher = HybridHashing();
        //    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        //        hyb_hasher.put(Couple(*it, *it));
        //    }
        //    BucketFactory<DepthBucket>::getInstance()->writeAll(hyb_hasher.getBuckets());
        //    hyb_hasher.clearBuckets();
        //    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        //        try {
        //            hyb_hasher.get(*it);
        //        } catch (string &e) {
        //            cout << e << endl;
        //        }
        //    }
        //    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        //    BucketFactory<DepthBucket>::getInstance()->removeAll();


        //    cout << "\n\n### Linear Hashing ###" << endl;
        //    tStart = clock();
        //    LinearHashing lin_hasher = LinearHashing();
        //    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        //        lin_hasher.put(Couple(*it, *it));
        //    }
        //    BucketFactory<ChainedBucket>::getInstance()->writeAll(lin_hasher.getBuckets());
        //    lin_hasher.clearBuckets();

        //    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        //        try {
        //            lin_hasher.get(*it);
        //        } catch (string &e) {
        //            cout << e << endl;
        //        }
        //    }
        //    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
        //    BucketFactory<ChainedBucket>::getInstance()->removeAll();

        cout << "Operation done successfully" << endl;
        C.disconnect ();
    } catch (const std::exception &e){
        cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}
