#include "multikeyExtendibleHashing.h"

MultikeyExtendibleHashing::MultikeyExtendibleHashing(string name, vector<int> keysRepartition)
    :ExtendibleHashing(name, keysRepartition)
{
}

Bucket *MultikeyExtendibleHashing::fetchBucket(size_t hash)
{
    numberBucketFetch++;
    return directory.getBucketFromName(hash);
}

vector<Couple> MultikeyExtendibleHashing::getCouples()
{
    vector<DepthBucket *>::iterator bucket;
    vector<Couple> couples;
    vector<DepthBucket *> buckets = directory.getBucketsFromName();
    numberBucketFetch += buckets.size();

    for (bucket = buckets.begin(); bucket != buckets.end(); ++bucket) {
        couples.insert(couples.end(), (*bucket)->getAllValues().begin(), (*bucket)->getAllValues().end());
    }

    return couples;
}
