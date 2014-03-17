#include "hashingMethod.h"
#include "directory.h"
using std::cout;
using std::endl;

int main()
{
    HashingMethod hasher = HashingMethod();
    Directory directory = Directory(hasher);
    size_t key = hasher.getHash("0");
    directory.putValue(key, "0");

//    cout << &directory.buckets << endl;
//    cout << &directory.buckets[0] << endl;
//    cout << directory.buckets.size() << endl;

//    DepthBucket& bucket = directory.getBucket(key);
//    cout << &bucket << endl;
//    directory.doubleSize();
//    cout << &directory.buckets << endl;
//    cout << &directory.buckets[0] << endl;
//    cout << directory.buckets.size() << endl;
    for (int i = 1; i < 10; i++) {
        directory.putValue(key, "0");
    }
    directory.putValue(key, "0");
}
