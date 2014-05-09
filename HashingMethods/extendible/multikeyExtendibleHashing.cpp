#include "multikeyExtendibleHashing.h"

MultikeyExtendibleHashing::MultikeyExtendibleHashing(string name, vector<int> keysRepartition)
    :ExtendibleHashing(name, keysRepartition)
{
    insertionLimit = 0.69 * Bucket::BUCKET_SIZE * 4;
}

Bucket *MultikeyExtendibleHashing::fetchBucket(size_t hash)
{
    Bucket *bucket = directory.getBucketFromName(hash);
    if (bucket->elements.size() > 0)
        numberBucketFetch++;

    return bucket;
}

void MultikeyExtendibleHashing::checkStructure()
{
    vector<DepthBucket *>::iterator bucket_it;
    vector<DepthBucket *> buckets = directory.getBucketsFromName();
    DepthBucket *bucket;
    string bucketName;
    insertedCouples.clear();
    numberBucketFetch = buckets.size();
    maxChainLength = 0;
    numberOverflowBuckets = 0;
    numberLongChain = 0;
    numberChain = 0;

    for (bucket_it = buckets.begin(); bucket_it != buckets.end(); ++bucket_it) {
        bucket = (*bucket_it);
        vector<Couple> values = bucket->elements;

        int chainCount = 0;
        while (bucket->hasNext()) {
            bucketName = bucket->nextName();
            bucket = BucketFactory<DepthBucket>::getInstance()->readBucket(bucketPath + bucketName);
            numberBucketFetch++;
            numberOverflowBuckets++;
            chainCount++;
            values.insert(values.end(), bucket->elements.begin(), bucket->elements.end());
        }

        if (chainCount > maxChainLength) {
            maxChainLength = chainCount;
        }
        if (chainCount > 0) {
            numberChain += 1;
        }
        if (chainCount > 1) {
            numberLongChain += 1;
        }
        insertedCouples.insert(insertedCouples.end(), values.begin(), values.end());
    }
    loadFactor = (double) insertedCouples.size() / (numberBucketFetch * Bucket::BUCKET_SIZE);

    cout << "global depth : " << directory.getGlobalDepth() << endl;
    cout << "number buckets : " << numberBucketFetch << endl;
    cout << "number overflow buckets : " << numberOverflowBuckets << endl;
    cout << "max chain length : " << maxChainLength << endl;
    cout << "number long chains : " << numberLongChain << endl;
    cout << "number chains : " << numberChain << endl;
    cout << "load factor : " << loadFactor << "\n" << endl;
}

bool MultikeyExtendibleHashing::canAddBHF()
{
    checkStructure();
    return (double) numberOverflowBuckets / numberBucketFetch >= 0.1;
}

void MultikeyExtendibleHashing::addBHF() {
    //    vector<Couple>::iterator couple;
    //    for (couple = insertedCouples.begin(); couple != insertedCouples.end(); ++couple) {
    //        for (int i = 0; i < couple->values.size(); i++) {
    //            if (keysRepartition[i] > 0) {
    //                if (!histograms[i][couple->values[i]]) {
    //                    histograms[i][couple->values[i]] = 1;
    //                } else {
    //                    histograms[i][couple->values[i]]++;
    //                }
    //            }
    //        }
    //    }

    //    for (int i = 0; i < keysRepartition.size(); i++) {
    //        if (keysRepartition[i] > 0) {
    //            int totalDistance = 0;
    //            map<string, int>::iterator it;
    //            for (it = histograms[i].begin(); it != histograms[i].end(); ++it) {
    //                totalDistance += it->second - 1;
    //            }
    //            cout << "column " << i << " distance to uniform distribution : " << (double) totalDistance / insertedCouples.size() << endl;
    //            cout << "column " << i << " number of different values : " << histograms[i].size() << endl;
    //        }
    //    }

    keysRepartition[0] += 1;
    globalDepthLimit += 1;
    insertionLimit *= 2;
    cout << "Adding BHF, global depth limit : " << globalDepthLimit << endl;
}

ostream& operator<<(ostream& strm, const MultikeyExtendibleHashing& hash)
{
    return hash.dump(strm);
}

