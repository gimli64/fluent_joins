#include "extendibleHashing.h"

ExtendibleHashing::ExtendibleHashing(string name, vector<int> BHFsRepartitions, vector<int> interleaveOrder)
    :HashTable(name, BHFsRepartitions, interleaveOrder)
{
    directory = Directory(this);
}

vector<string> ExtendibleHashing::getValue(size_t hash, string key)
{
    try {
        return directory.getValue(hash, key);
    } catch (string &e) {
        throw e;
    }
}

void ExtendibleHashing::putCouple(size_t hash, Couple couple)
{
    directory.putCouple(hash, couple);
}

void ExtendibleHashing::getBuckets(vector<size_t> &setHashes, vector<int> &sizes, set<Bucket *> &buckets)
{
    vector<size_t> hashes;
    getHashes(setHashes, sizes, hashes);
    for (int i = 0; i < hashes.size(); i++) {
        buckets.insert((Bucket*) directory.getBucket(hashes[i]));
    }
}

int ExtendibleHashing::getNumberBuckets()
{
    vector<Bucket *> buckets = directory.getBuckets();
    set<Bucket *> uniqueBuckets(buckets.begin(), buckets.end());
    return uniqueBuckets.size();
}

void ExtendibleHashing::printState()
{
    vector<Bucket *> buckets = directory.getBuckets();
    set<Bucket *> uniqueBuckets(buckets.begin(), buckets.end());
    double loadFactor = (double) numberItems / (uniqueBuckets.size() * Bucket::BUCKET_SIZE);
    double distanceToUniform = 0.0;

    Bucket* bucket;
    for(set<Bucket*>::iterator it = uniqueBuckets.begin(); it != uniqueBuckets.end(); ++it) {
        bucket = *it;
        distanceToUniform += pow(((double) bucket->size() / Bucket::BUCKET_SIZE) - loadFactor, 2.0);
    }

    cout << "global depth : " << directory.getGlobalDepth() << endl;
    cout << "directory page size : " << directory.pageSize() << endl;
    cout << "keys repartition : [";
    for (int i = 0; i < BHFsRepartitions.size(); i++) {
        cout << BHFsRepartitions[i];
        if (i < BHFsRepartitions.size() - 1)
            cout << ",";
    }
    cout << "]" << endl;
    BucketFactory<Bucket>::getInstance()->printState();
    cout << "distance to uniform : " << distanceToUniform << endl;
    cout << "load factor : " << loadFactor << endl << endl;
}

void ExtendibleHashing::addBHF() {
    if (BHFsRepartitions.size() == 1) {
        BHFsRepartitions[0] += 1;
        interleaveOrder.push_back(0);
    } else {
        int minBHFsAllocated = interleaveOrder.size();
        int minBHFsAllocatedIndex = 0;
        for (int i = 0; i < BHFsRepartitions.size(); i++) {
            if (BHFsRepartitions[i] > 0 && BHFsRepartitions[i] < minBHFsAllocated) {
                minBHFsAllocated = BHFsRepartitions[i];
                minBHFsAllocatedIndex = i;
            }
        }
        cout << "Allocating BHF to " << minBHFsAllocatedIndex << endl;
        BHFsRepartitions[minBHFsAllocatedIndex] += 1;
        interleaveOrder.push_back(minBHFsAllocatedIndex);
    }
}
