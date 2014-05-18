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

DepthBucket *ExtendibleHashing::fetchBucket(size_t hash)
{
    DepthBucket *bucket = directory.fetchBucket(hash);
    if (bucket->size() > 0) {
        numberBucketFetch += bucket->getChainCount();
    }

    return bucket;
}

vector<Couple> ExtendibleHashing::fetchAllCouples()
{
    vector<Couple> couples;
    vector<DepthBucket *> buckets = directory.getBuckets();
    for (int i = 0; i < buckets.size(); i++) {
        vector<Couple> values = buckets[i]->getAllValues();
        couples.insert(couples.end(), values.begin(), values.end());
    }
    numberBucketFetch += buckets.size();
    return couples;
}

vector<Couple> ExtendibleHashing::fetchCouples(size_t keyHash, int keyHashSize, int position, size_t keyHash2, int keyHashSize2, int position2)
{
    DepthBucket *bucket;
    vector<Couple> couples;
    vector<size_t> hashes;
    getHashes(keyHash, keyHashSize, position, keyHash2, keyHashSize2, position2, hashes);
    for (int i = 0; i < hashes.size(); i++) {
        bucket = fetchBucket(hashes[i]);
        vector<Couple> values = bucket->getAllValues();
        couples.insert(couples.end(), values.begin(), values.end());
    }
    delete bucket;
    return couples;
}


void ExtendibleHashing::reset()
{
    directory.reset();
}

string ExtendibleHashing::className() const
{
    return "ExtendibleHashing ";
}

ostream& ExtendibleHashing::dump(ostream& strm) const
{
    const void * address = static_cast<const void*>(this);
    stringstream ss;
    ss << address;
    ostream& output = strm;
    output << className() + ss.str() + " : \n";
    output << "## " << directory;
    return output;
}

void ExtendibleHashing::printState()
{
    cout << "global depth : " << directory.getGlobalDepth() << endl;
    BucketFactory<DepthBucket>::getInstance()->printState();
    cout << "keys repartition : [";
    for (int i = 0; i < keysRepartition.size(); i++)
        cout << keysRepartition[i] << ", ";
    cout << "]" << endl;
    cout << "load factor : " << (double) numberItems / (BucketFactory<DepthBucket>::getInstance()->getNumberBuckets() * Bucket::BUCKET_SIZE) << "\n" << endl;
}

void ExtendibleHashing::splitChainedBuckets()
{
    vector<DepthBucket *> buckets = directory.getBuckets();
    int localDepthTotal = 0;
    int minLocalDepth = directory.getGlobalDepth();
    int numberBucket13 = 0;
    int numberBucket12 = 0;
    for (int i = 0; i < buckets.size(); i++) {
        localDepthTotal += buckets[i]->getLocalDepth();
        if (buckets[i]->getLocalDepth() < minLocalDepth)
            minLocalDepth = buckets[i]->getLocalDepth();
        //        if (buckets[i]->getChainCount() > 1) {
        //            directory.split(buckets[i]);
        //        }
        if (buckets[i]->getLocalDepth() == 12)
            numberBucket12 += 1;
        if (buckets[i]->getLocalDepth() == 13)
            numberBucket13 += 1;
    }
    cout << numberBucket12 << endl;
    cout << numberBucket13 << endl;
    cout << minLocalDepth << endl;
    cout << (double) localDepthTotal / buckets.size() << endl;
}

bool ExtendibleHashing::addBHF() {
    double maxNeededBHFRatio = 0.0;
    int maxNeededBHFRatioIndex = 0;
    int totalKeysRepartition = 0;

    for (int i = 0; i < keysRepartition.size(); i++) {
        if (keysRepartition[i] > 0) {
            double totalDistance = 0.0;
            map<string, int>::iterator it;
            for (it = histograms[i].begin(); it != histograms[i].end(); ++it) {
                totalDistance += abs(((double) it->second / numberItems) - (1.0 / histograms[i].size()));
            }

            int neededNumberBuckets = (int) round((double) histograms[i].size() / Bucket::BUCKET_SIZE);
            int neededBHF = 1;
            while (neededNumberBuckets > 1) {
                neededNumberBuckets >>= 1;
                neededBHF += 1;
            }
            double neededBHFRatio = ((double) neededBHF - keysRepartition[i])/* * (1 - totalDistance)*/;
            if (neededBHFRatio > maxNeededBHFRatio) {
                maxNeededBHFRatio = neededBHFRatio;
                maxNeededBHFRatioIndex = i;
            }
            totalKeysRepartition += keysRepartition[i];
        }
    }

    if (maxNeededBHFRatio == 0.0 or totalKeysRepartition > directory.getGlobalDepth()) {
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

ostream& operator<<(ostream& strm, const ExtendibleHashing& hash)
{
    return hash.dump(strm);
}
