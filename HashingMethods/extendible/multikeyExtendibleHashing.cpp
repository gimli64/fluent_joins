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

void MultikeyExtendibleHashing::checkStructure()
{
    vector<DepthBucket *>::iterator bucket_it;
//    vector<DepthBucket *> buckets = directory.getBucketsFromName();
    vector<DepthBucket *> buckets = directory.getBuckets();
    DepthBucket *bucket;
//    string bucketName;
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
//            bucketName = bucket->nextName();
//            bucket = BucketFactory<DepthBucket>::getInstance()->readBucket(bucketPath + bucketName);
            bucket = bucket->next();
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

double MultikeyExtendibleHashing::updateStructure()
{
    double numberUpdates = 0.0;

    if (maxChainLength > 1 and ((double) numberLongChain / numberChain >= 0.5)) {
        Bucket::BUCKET_SIZE *= 2;
        numberUpdates += 1;
        cout << "Doubling bucket size : " << Bucket::BUCKET_SIZE << endl;
    } /*else if (maxChainLength > 1) {
        Bucket::BUCKET_SIZE = int(Bucket::BUCKET_SIZE * 1.5);
        numberUpdates += 1.5;
        cout << "Raising bucket size by 50% : " << Bucket::BUCKET_SIZE << endl;
    }*/

    if (abs(loadFactor - 0.69) >= 0.08 or ((double) numberOverflowBuckets / numberBucketFetch >= 0.33)) {
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
        numberUpdates += 1;
        cout << "Adding BHF, global depth : " << globalDepthLimit + 1 << endl;
    }

    if (numberUpdates == 0)
        insertedCouples.clear();
    return numberUpdates;
}

void MultikeyExtendibleHashing::reInsertCouples()
{
    cout << "\nReinserting " << insertedCouples.size() << " values " << endl;
    BucketFactory<DepthBucket>::getInstance()->reset();
    directory = Directory(this);
    for (int i = 0; i < insertedCouples.size(); i++) {
        putMultikey(insertedCouples[i]);
    }
    insertedCouples.clear();
    checkStructure();
    insertedCouples.clear();
}

ostream& operator<<(ostream& strm, const MultikeyExtendibleHashing& hash)
{
    return hash.dump(strm);
}

