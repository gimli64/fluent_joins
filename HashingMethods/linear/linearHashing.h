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

    vector<ChainedBucket*> getBuckets();
    void clearBuckets();

private:
    static const double SPLIT_RATIO;
    int level;
    int nextSplitIndex;
    int initialNumberBuckets;
    int bucketCapacity;
    vector<ChainedBucket*> buckets;
    vector<string> bucketNames;
    BucketFactory<ChainedBucket> *factory;

    virtual vector<string> getValue(size_t hash, string key);
    virtual void putCouple(size_t hash, Couple couple);

    ChainedBucket *getBucket(size_t hash);
    ChainedBucket *getBucketFromName(size_t hash);

    double getRatio();
    void incrementSplitIndex();
    void split();
};

ostream& operator<<(ostream&, const LinearHashing&);

#endif // LINEARHASHING_H
