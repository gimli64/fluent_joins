#include "multikeyExtendibleHashing.h"

MultikeyExtendibleHashing::MultikeyExtendibleHashing(string name, vector<int> keysRepartition)
    :ExtendibleHashing(name, keysRepartition)
{
}

Bucket *MultikeyExtendibleHashing::fetchBucket(size_t hash)
{
//    numberBucketFetch++;
//    return directory.getBucketFromName(hash);

    Bucket *bucket = directory.getBucketFromName(hash);
    if (bucket->getAllValues().size() > 0)
        numberBucketFetch++;

    return bucket;
}

vector<Couple> MultikeyExtendibleHashing::getCouples()
{
    vector<DepthBucket *>::iterator bucket;
    vector<Couple> couples;
    vector<DepthBucket *> buckets = directory.getBucketsFromName();
    numberBucketFetch += buckets.size();
    totalBucketSize = 0;
    maxChainLength = 0;
    numberOverflowBuckets = 0;

    for (bucket = buckets.begin(); bucket != buckets.end(); ++bucket) {
        vector<Couple> values = (*bucket)->getAllValues();
        totalBucketSize += values.size();
        if (values.size() > Bucket::BUCKET_SIZE) {
            numberOverflowBuckets++;

            if (values.size() / Bucket::BUCKET_SIZE > maxChainLength and values.size() % Bucket::BUCKET_SIZE > 0) {
                maxChainLength = values.size() / Bucket::BUCKET_SIZE;
            }
        }
        if (values.size() == 0) {
            numberEmptyBuckets++;
        }
        couples.insert(couples.end(), values.begin(), values.end());
    }

    return couples;
}

void MultikeyExtendibleHashing::printState()
{
    getCouples();
    cout << "empty buckets : " << numberEmptyBuckets << endl;
    cout << "global depth : " << directory.getGlobalDepth() << endl;
    cout << "number dir entries : " << pow(2.0, (double) (directory.getGlobalDepth())) << endl;
    cout << "max chain length : " << maxChainLength << endl;
    cout << "number overflow buckets : " << numberOverflowBuckets << endl;
    cout << "load factor : " << (double) totalBucketSize / numberBucketFetch << endl;
}

ostream& operator<<(ostream& strm, const MultikeyExtendibleHashing& hash)
{
    return hash.dump(strm);
}

