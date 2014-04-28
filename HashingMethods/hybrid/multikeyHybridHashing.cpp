#include "multikeyHybridHashing.h"

MultikeyHybridHashing::MultikeyHybridHashing(string name, vector<int> keysRepartition)
    :HybridHashing(name, keysRepartition)
{
    level = leftMostBitIndex;
}

Bucket *MultikeyHybridHashing::fetchBucket(size_t hash)
{
    numberBucketFetch++;
    return getHybridDirectory(hash).getBucketFromName(hash);
}

HybridDirectory &MultikeyHybridHashing::getHybridDirectory(size_t hash)
{
    hash = getLeftMostBits(hash);
    int pageIndex = hash & mask;
    if (pageIndex < nextSplitIndex) {
        pageIndex = hash & (mask | (1 << (leftMostBitIndex - level)));
    }

    return directories.at(pageIndex);
}

size_t MultikeyHybridHashing::getLeftMostBits(size_t hash)
{
    size_t newKey = 0;
    size_t bit = 0;
    for (int i = leftMostBitIndex; i >= level; i--) {
        bit = (hash & (1 << i)) >> i;
        newKey |= (bit << (leftMostBitIndex - i));
    }
    return newKey;
}

void MultikeyHybridHashing::incrementSplitIndex()
{
    nextSplitIndex++;
    if (nextSplitIndex == initialNumberDirectories) {  // we start a new round
        nextSplitIndex = 0;
        mask |= (1 << (leftMostBitIndex - level));
        level -= 1;
        initialNumberDirectories = directories.size();
    }
}

void MultikeyHybridHashing::split()
{
    HybridDirectory directoryToSplit = directories.at(nextSplitIndex);
    vector<Couple> values = directoryToSplit.popAllValues();
    numberDirEntries -= directoryToSplit.getSize();
    numberItems -= values.size();

    directories.at(nextSplitIndex) = HybridDirectory(this);
    directories.push_back(HybridDirectory(this));
    incrementSplitIndex();

    for (vector<Couple>::iterator it = values.begin(); it != values.end(); ++it)
        putMultikey(*it);
}


vector<Couple> MultikeyHybridHashing::getCouples()
{
    vector<DepthBucket*> buckets;
    vector<DepthBucket *>::iterator bucket;
    vector<Couple> couples;

    for (int i = 0; i < directories.size(); i++) {
        buckets = directories.at(i).getBucketsFromName();
        numberBucketFetch += buckets.size();

        for (bucket = buckets.begin(); bucket != buckets.end(); ++bucket) {
            vector<Couple> values = (*bucket)->getAllValues();
            if (values.size() == 0)
                numberEmptyBuckets++;
            couples.insert(couples.end(), values.begin(), values.end());
        }

    }

    return couples;
}

ostream& operator<<(ostream& strm, const MultikeyHybridHashing& dir)
{
    return dir.dump(strm);
}

