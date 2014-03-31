#include "comparer.h"
#include <time.h>

int main()
{
    Comparer comparer = Comparer();
    vector<string> values = comparer.fetchData("comparison_data");
    clock_t tStart = clock();

    cout << "Comparing three dynamic hashing algorightms" << endl;

    cout << "\n\n### Extendible Hashing ###" << endl;
    ExtendibleHashing ext_hasher = ExtendibleHashing();
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        ext_hasher.put(Couple(*it, *it));
    }
//    cout << ext_hasher << endl;
    BucketFactory<DepthBucket>::getInstance()->writeAll(ext_hasher.getBuckets());
    ext_hasher.clearBuckets();

    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        try {
            ext_hasher.get(*it);
        } catch (string &e) {
            cout << e << endl;
        }
    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    BucketFactory<DepthBucket>::getInstance()->removeAll();


    cout << "\n\n### Hybrid Hashing ###" << endl;
    tStart = clock();
    HybridHashing hyb_hasher = HybridHashing();
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        hyb_hasher.put(Couple(*it, *it));
    }
//    cout << hyb_hasher << endl;
    BucketFactory<DepthBucket>::getInstance()->writeAll(hyb_hasher.getBuckets());
    hyb_hasher.clearBuckets();
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        try {
            hyb_hasher.get(*it);
        } catch (string &e) {
            cout << e << endl;
        }
    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    BucketFactory<DepthBucket>::getInstance()->removeAll();


    cout << "\n\n### Linear Hashing ###" << endl;
    tStart = clock();
    LinearHashing lin_hasher = LinearHashing();
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        lin_hasher.put(Couple(*it, *it));
    }
//    cout << lin_hasher << endl;
    BucketFactory<ChainedBucket>::getInstance()->writeAll(lin_hasher.getBuckets());
    lin_hasher.clearBuckets();

    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        try {
            lin_hasher.get(*it);
        } catch (string &e) {
            cout << e << endl;
        }
    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
    BucketFactory<ChainedBucket>::getInstance()->removeAll();
}
