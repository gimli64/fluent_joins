#include "comparer.h"
#include <time.h>

int main()
{
    Comparer comparer = Comparer();
//    int who = RUSAGE_SELF;
//    struct rusage usage;
    vector<string> values = comparer.fetchData("comparison_data");
    cout << "Comparing three dynamic hashing algorightms" << endl;

    cout << "\n\n### Extendible Hashing ###" << endl;
    ExtendibleHashing ext_hasher = ExtendibleHashing();
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        ext_hasher.put(*it);
    }

//    cout << ext_hasher << endl;
    clock_t tStart = clock();
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        try {
            ext_hasher.get(*it);
        } catch (string &e) {
            cout << e << endl;
        }
    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
////    getrusage(who, &usage);
////    printf("Resource usage : %ld kB \n", usage.ru_maxrss);

    BucketFactory<DepthBucket>::getInstance()->removeAll();

    cout << "\n\n### Hybrid Hashing ###" << endl;
    HybridHashing hyb_hasher = HybridHashing();
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        hyb_hasher.put(*it);
//        cout << hyb_hasher << endl;
    }
//    cout << hyb_hasher << endl;
    tStart = clock();
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        try {
            hyb_hasher.get(*it);
        } catch (string &e) {
            cout << e << endl;
        }
    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
////    getrusage(who, &usage);
////    printf("Resource usage : %ld kB \n", usage.ru_maxrss);

    BucketFactory<ChainedBucket>::getInstance()->removeAll();

    cout << "\n\n### Linear Hashing ###" << endl;
    LinearHashing lin_hasher = LinearHashing();
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        lin_hasher.put(*it);
    }

//    cout << lin_hasher << endl;
    tStart = clock();
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        try {
            lin_hasher.get(*it);
        } catch (string &e) {
            cout << e << endl;
        }
    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
////    getrusage(who, &usage);
////    printf("Resource usage : %ld kB \n", usage.ru_maxrss);
}
