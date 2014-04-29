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
        vector<Couple> values = (*bucket)->getAllValues();
        if (values.size() == 0) {
            cout << (*bucket)->name << endl;
            numberEmptyBuckets++;
        }
        couples.insert(couples.end(), values.begin(), values.end());
    }

    return couples;
}

ostream& operator<<(ostream& strm, const MultikeyExtendibleHashing& hash)
{
    return hash.dump(strm);
}

