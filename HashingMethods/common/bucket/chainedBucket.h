#ifndef CHAINEDBUCKET_H
#define CHAINEDBUCKET_H
#include "bucket.h"

class ChainedBucket : public Bucket
{
public:
    ChainedBucket();
    ChainedBucket(HashingMethod hasher);

    string* getValue(string value);
    void putValue(string value);
    int getChainCount();

private:
    ChainedBucket* nextBucket;
};

#endif // CHAINEDBUCKET_H
