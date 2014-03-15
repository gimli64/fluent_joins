#include "hashingMethod.h"
#include "directory.h"
using std::cout;
using std::endl;

int main()
{
    HashingMethod hasher = HashingMethod();
    Directory dir = Directory(hasher);
    vector<DepthBucket>& buckets = dir.getBuckets();
    cout << &buckets << endl;
}
