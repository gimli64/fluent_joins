#include "common/bucket/chainedBucket.h"
#include "common/bucket/depthBucket.h"
#include "common/directory/directory.h"
#include "common/directory/chainedDirectory.h"
#include "common/hashingMethod.h"
#include "linear/linearHashing.h"
#include "extendible/extendibleHashing.h"
#include "hybrid/hybridHashing.h"

using namespace std;

int main()
{
    HybridHashing hasher = HybridHashing();
    BucketFactory<DepthBucket> *factory = BucketFactory<DepthBucket>::getInstance();
    cout << hasher << endl;
    cout << factory->getNumberBuckets() << endl;
    hasher.put("0");
    cout << hasher << endl;
    cout << factory->getNumberBuckets() << endl;
    hasher.put("0");
    cout << hasher << endl;
    cout << factory->getNumberBuckets() << endl;
    hasher.put("0");
    cout << hasher << endl;
    cout << factory->getNumberBuckets() << endl;
    hasher.put("0");
    cout << hasher << endl;
    cout << factory->getNumberBuckets() << endl;
    hasher.put("0");
    cout << hasher << endl;
    cout << factory->getNumberBuckets() << endl;
    hasher.put("0");
    cout << hasher << endl;
    cout << factory->getNumberBuckets() << endl;
    hasher.put("0");
    cout << hasher << endl;
    cout << factory->getNumberBuckets() << endl;
    hasher.put("0");
    cout << hasher << endl;
    cout << factory->getNumberBuckets() << endl;
    hasher.put("0");
    cout << hasher << endl;
    cout << factory->getNumberBuckets() << endl;
    hasher.put("0");
    cout << hasher << endl;
    cout << factory->getNumberBuckets() << endl;
}
