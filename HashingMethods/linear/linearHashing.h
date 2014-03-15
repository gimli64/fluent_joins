#ifndef LINEARHASHING_H
#define LINEARHASHING_H
#include "hashingMethod.h"
#include "chainedBucket.h"

class LinearHashing : public HashingMethod
{
public:
    LinearHashing();

    string* get(size_t key, string value);
    void put(size_t key, string value);

private:
    static const double SPLIT_RATIO;
    int level;
    int nextSplitIndex;
    int initialNumberBuckets;
    int bucketCapacity;
    vector<ChainedBucket> buckets;

    ChainedBucket& getBucket(size_t key);
    double getRatio();
    void incrementSplitIndex();
    void split();
};

#endif // LINEARHASHING_H
