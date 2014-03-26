#ifndef LINEARHASHING_H
#define LINEARHASHING_H

#include "common/hashingMethod.h"
#include "common/bucket/chainedBucket.h"
#include "common/bucket/bucketFactory.h"

using namespace std;

class LinearHashing : public HashingMethod
{
public:
    LinearHashing();
    virtual ostream& dump(ostream& strm) const;

private:
    static const double SPLIT_RATIO;
    int level;
    int nextSplitIndex;
    int initialNumberBuckets;
    int bucketCapacity;
    vector<string> buckets;
    BucketFactory<ChainedBucket>* factory;

    virtual string getValue(size_t hash, string key);
    virtual void putCouple(size_t hash, Couple couple);
    ChainedBucket *getBucket(size_t hash);
    double getRatio();
    void incrementSplitIndex();
    void split();
};

ostream& operator<<(ostream&, const LinearHashing&);

#endif // LINEARHASHING_H
