#include "hashingMethod.h"
#include "directory.h"
#include "chainedBucket.h"
using std::cout;
using std::endl;

int main()
{
    DepthBucket bucket = DepthBucket();
    bucket.putValue("0");
    for (int i = 0; i < Bucket::BUCKET_SIZE; i++) {
        bucket.putValue("0");
    }
    cout << bucket << endl;
}
