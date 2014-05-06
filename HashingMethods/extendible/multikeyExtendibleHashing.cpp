#include "multikeyExtendibleHashing.h"

MultikeyExtendibleHashing::MultikeyExtendibleHashing(string name, vector<int> keysRepartition)
    :ExtendibleHashing(name, keysRepartition)
{
}

Bucket *MultikeyExtendibleHashing::fetchBucket(size_t hash)
{
    Bucket *bucket = directory.getBucketFromName(hash);
    if (bucket->getAllValues().size() > 0)
        numberBucketFetch++;

    return bucket;
}

vector<Couple> MultikeyExtendibleHashing::getCouples()
{
    vector<DepthBucket *>::iterator bucket_it;
    vector<DepthBucket *> buckets = directory.getBucketsFromName();
    DepthBucket *bucket;
    numberBucketFetch += buckets.size();
    totalBucketSize = 0;
    maxChainLength = 0;
    numberOverflowBuckets = 0;
    vector<Couple> couples;
    string bucketName;

    for (bucket_it = buckets.begin(); bucket_it != buckets.end(); ++bucket_it) {
        bucket = (*bucket_it);
        vector<Couple> values = bucket->elements;
        totalBucketSize += values.size();

        int chainCount = 0;
        while (bucket->hasNext()) {
            bucketName = bucket->nextName();
            bucket = BucketFactory<DepthBucket>::getInstance()->readBucket(bucketPath + bucketName);
            numberBucketFetch++;
            numberOverflowBuckets++;
            chainCount++;
            totalBucketSize += bucket->elements.size();
            values.insert(values.end(), bucket->elements.begin(), bucket->elements.end());
        }

        if (chainCount > maxChainLength) {
            maxChainLength = chainCount;
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
    //    cout << "empty buckets : " << numberEmptyBuckets << endl;
    cout << "global depth : " << directory.getGlobalDepth() << endl;
    cout << "number dir entries : " << pow(2.0, (double) (directory.getGlobalDepth())) << endl;
    cout << "max chain length : " << maxChainLength << endl;
    cout << "number overflow buckets : " << numberOverflowBuckets << endl;
    cout << "load factor : " << (double) totalBucketSize / (numberBucketFetch * Bucket::BUCKET_SIZE) << endl;
}

ostream& operator<<(ostream& strm, const MultikeyExtendibleHashing& hash)
{
    return hash.dump(strm);
}

