#include "comparer.h"
#include <time.h>

int main()
{
    Comparer comparer = Comparer();
    vector<string> values = comparer.fetchData("comparison_data");
    cout << "Comparing three dynamic hashing algorightms" << endl;

    clock_t tStart = clock();
    cout << "### Extendible Hashing ###" << endl;
    ExtendibleHashing ext_hasher = ExtendibleHashing();
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        ext_hasher.put(*it);
    }
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        if (ext_hasher.get(*it) == (string*) NULL)
            cout << "Error" << endl;
    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    tStart = clock();
    cout << "### Linear Hashing ###" << endl;
    LinearHashing lin_hasher = LinearHashing();
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        lin_hasher.put(*it);
    }
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        if (lin_hasher.get(*it) == (string*) NULL)
            cout << "Error" << endl;
    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);

    tStart = clock();
    cout << "### Hybrid Hashing ###" << endl;
    HybridHashing hyb_hasher = HybridHashing();
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        hyb_hasher.put(*it);
    }
    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        if (hyb_hasher.get(*it) == (string*) NULL)
            cout << "Error" << endl;
    }
    printf("Time taken: %.2fs\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
}
