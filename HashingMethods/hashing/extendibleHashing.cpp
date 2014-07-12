#include "extendibleHashing.h"

ExtendibleHashing::ExtendibleHashing(string name, vector<int> BHFsRepartitions)
    :directory(Directory(this)), HashTable(name, BHFsRepartitions)
{
}

vector<string> ExtendibleHashing::getValue(size_t hash)
{
    try {
        return directory.getValue(hash);
    } catch (string &e) {
        throw e;
    }
}

void ExtendibleHashing::putCouple(size_t hash, Couple couple)
{
    directory.putCouple(hash, couple);
}

set<Bucket*> ExtendibleHashing::getBuckets(size_t keyHash, int keyHashSize, int position)
{
    set<Bucket*> buckets;
    vector<size_t> hashes;
    getHashes(keyHash, keyHashSize, position, hashes);
    for (int i = 0; i < hashes.size(); i++) {
        buckets.insert((Bucket*) directory.getBucket(hashes[i]));
    }
    return buckets;
}

void ExtendibleHashing::printState()
{
    cout << "global depth : " << directory.getGlobalDepth() << endl;
    cout << "directory page size : " << directory.pageSize() << endl;
    BucketFactory<Bucket>::getInstance()->printState();
    cout << "keys repartition : [";
    for (int i = 0; i < BHFsRepartitions.size(); i++) {
        cout << BHFsRepartitions[i];
        if (i < BHFsRepartitions.size() - 1)
            cout << ",";
    }
    cout << "]" << endl;
    cout << "load factor : ";
    cout << (double) numberItems / (BucketFactory<Bucket>::getInstance()->getNumberBuckets() * Bucket::BUCKET_SIZE);
    cout << endl << endl;
//    cout << directory << endl;
}
