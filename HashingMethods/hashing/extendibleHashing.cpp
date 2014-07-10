#include "extendibleHashing.h"

ExtendibleHashing::ExtendibleHashing(string name, vector<int> keysRepartition, vector<int> interleaveOrder)
    :directory(Directory(this)), HashTable(name, keysRepartition, interleaveOrder)
{
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

vector<Couple> ExtendibleHashing::getCouples(size_t keyHash, int keyHashSize, int position)
{
//    Bucket *bucket;
//    vector<Couple> couples;
//    vector<size_t> hashes;
//    getHashes(keyHash, keyHashSize, position, hashes);
//    for (int i = 0; i < hashes.size(); i++) {
//        bucket = (Bucket*) directory.getBucket(hashes[i]);
//        vector<Couple> values = bucket->getAllValues();
//        couples.insert(couples.end(), values.begin(), values.end());
//    }
//    return couples;
}

void ExtendibleHashing::dimensionStats(int position)
{
//    int keyHashSize = keysRepartition[position];
//    map<Bucket*, bool> bucketFetched;
//    double distance = 0;
//    double load_factor = ((double) numberItems / (directory.getBuckets().size() * Bucket::BUCKET_SIZE));
//    Bucket *bucket;

//    ofstream distribution_file;
//    distribution_file.open ("/Users/gimli/projects/fluent_joins/stats/bucket_distribution");

//    for (size_t keyHash = 0; keyHash < (int) pow(2.0, (double) keyHashSize); keyHash++) {
//        vector<size_t> hashes;
//        vector<Bucket*> buckets;
//        getHashes(keyHash, keyHashSize, position, hashes);
//        for (int i = 0; i < hashes.size(); i++) {
//            bucket = directory.getBucket(hashes[i]);
//            if (!bucketFetched[bucket]) {
//                bucketFetched[bucket] = true;
//                buckets.push_back(directory.getBucket(hashes[i]));
//            }
//        }
//        for (vector<Bucket*>::iterator bucket = buckets.begin(); bucket != buckets.end(); ++bucket) {
//            distance += abs(((double) (*bucket)->size() / Bucket::BUCKET_SIZE) - load_factor);
//            distribution_file <<  (*bucket)->size() <<  "\n";
//        }
////        cout << "Maximum load factor in partition " << keyHash << " : " << maxLoadFactor << endl;
//    }
//    distribution_file.close();
//    cout << "Distance for table " << position << " : " << distance << endl;
//    cout << "\n" << endl;
}

void ExtendibleHashing::printState()
{
    cout << "global depth : " << directory.getGlobalDepth() << endl;
    BucketFactory<Bucket>::getInstance()->printState();

//    vector<Bucket *> buckets = directory.getBuckets();
    cout << "keys repartition : [";
    for (int i = 0; i < keysRepartition.size(); i++) {
        cout << keysRepartition[i];
        if (i < keysRepartition.size() - 1)
            cout << ",";
    }
    cout << "]\n" << endl;

    cout << directory << endl;
//    cout << "load factor : " << (double) numberItems / (buckets.size() * Bucket::BUCKET_SIZE) << "\n" << endl;
}

bool ExtendibleHashing::addBHF() {
    double maxNeededBHFRatio = 0.0;
    int maxNeededBHFRatioIndex = 0;
    int totalKeysRepartition = 0;

    for (int i = 0; i < keysRepartition.size(); i++) {
        totalKeysRepartition += keysRepartition[i];
    }

    for (int i = 0; i < keysRepartition.size(); i++) {
        if (keysRepartition[i] > 0) {
            int neededNumberBuckets = (double) histograms[i].size() / (Bucket::BUCKET_SIZE);
            double neededBHFRatio = ((double) log(neededNumberBuckets) / log(2) - keysRepartition[i]);
//            neededBHFRatio *= (totalKeysRepartition / keysRepartition[i]);
            cout << "column " << i << " neededBHFRatio : " << neededBHFRatio << endl;
            if (neededBHFRatio > maxNeededBHFRatio) {
                maxNeededBHFRatio = neededBHFRatio;
                maxNeededBHFRatioIndex = i;
            }
        }
    }

    if (maxNeededBHFRatio <= 0.0 or totalKeysRepartition > directory.getDepth()) {
        cout << "Not Adding BHF " << endl;
        return false;
    } else {
        globalDepthLimit += 1;
        keysRepartition[maxNeededBHFRatioIndex] += 1;
        interleaveOrder.push_back(maxNeededBHFRatioIndex);
        cout << "Adding BHF on column " << maxNeededBHFRatioIndex << endl;
        return true;
    }
}
