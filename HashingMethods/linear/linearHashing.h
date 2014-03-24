#ifndef LINEARHASHING_H
#define LINEARHASHING_H

#include "common/hashingMethod.h"
#include "common/bucket/chainedBucket.h"
#include "common/bucket/bucketFactory.h"

using namespace std;

class LinearHashing : public HashingMethod
{
public:
    static LinearHashing *getInstance();
    virtual ostream& dump(ostream& strm) const;

private:
    static const double SPLIT_RATIO;
    int level;
    int nextSplitIndex;
    int initialNumberBuckets;
    int bucketCapacity;
    vector<string> buckets;
    BucketFactory<ChainedBucket>* factory;
    static LinearHashing *instance;

    LinearHashing();
    virtual string getValue(size_t key, string value);
    virtual void putValue(size_t key, string value);
    ChainedBucket *getBucket(size_t key);
    double getRatio();
    void incrementSplitIndex();
    void split();
};

ostream& operator<<(ostream&, const LinearHashing&);

#endif // LINEARHASHING_H
