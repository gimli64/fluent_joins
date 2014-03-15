#ifndef HYBRIDHASHING_H
#define HYBRIDHASHING_H
#include "hashingMethod.h"
#include "chainedDirectory.h"
#include "depthBucket.h"

class HybridHashing : public HashingMethod
{
public:
    HybridHashing();
    string* get(size_t key, string value);
    void put(size_t key, string value);

private:
    static const double SPLIT_RATIO;
    int level;
    int mask;
    int nextSplitIndex;
    int initialNumberDirectories;
    int bucketCapacity;
    vector<ChainedDirectory> directories;

    ChainedDirectory& getChainedDirectory(size_t key);
    int getLeftMostBits(size_t key);
    void incrementSplitIndex();
    double getRatio();
    void split();
};

#endif // HYBRIDHASHING_H
