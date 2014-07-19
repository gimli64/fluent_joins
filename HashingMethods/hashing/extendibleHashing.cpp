#include "extendibleHashing.h"

ExtendibleHashing::ExtendibleHashing(string name, vector<int> BHFsRepartitions, vector<int> interleaveOrder)
    :directory(Directory(this)), HashTable(name, BHFsRepartitions, interleaveOrder)
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

void ExtendibleHashing::addBHF() {
//    double maxNeededBHFRatio = 0.0;
//    int maxNeededBHFRatioIndex = 0;
//    int totalBHFsRepartitions = 0;

//    for (int i = 0; i < BHFsRepartitions.size(); i++) {
//        totalBHFsRepartitions += BHFsRepartitions[i];
//    }

//    for (int i = 0; i < BHFsRepartitions.size(); i++) {
//        if (BHFsRepartitions[i] > 0) {
//            int neededNumberBuckets = (double) histograms[i].size() / (Bucket::BUCKET_SIZE);
//            double neededBHFRatio = ((double) log(neededNumberBuckets) / log(2) - BHFsRepartitions[i]);
////            neededBHFRatio *= (totalBHFsRepartitions / BHFsRepartitions[i]);
//            cout << "column " << i << " neededBHFRatio : " << neededBHFRatio << endl;
//            if (neededBHFRatio > maxNeededBHFRatio) {
//                maxNeededBHFRatio = neededBHFRatio;
//                maxNeededBHFRatioIndex = i;
//            }
//        }
//    }

//    if (maxNeededBHFRatio <= 0.0 or totalBHFsRepartitions > directory.getDepth()) {
//        cout << "Not Adding BHF " << endl;
//    } else {
//        BHFsRepartitions[maxNeededBHFRatioIndex] += 1;
//        interleaveOrder.push_back(maxNeededBHFRatioIndex);
//        cout << "Adding BHF on column " << maxNeededBHFRatioIndex << endl;
//    }

    if (BHFsRepartitions.size() == 1) {
        BHFsRepartitions[0] += 1;
        interleaveOrder.push_back(0);
    } else {
        int maxNumberPagesIndex = Manager::getInstance()->performAllJoins();

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

//        cout << "Allocating BHF to " << maxNumberPagesIndex << endl;
//        BHFsRepartitions[maxNumberPagesIndex] += 1;
//        interleaveOrder.push_back(maxNumberPagesIndex);
    }
}
