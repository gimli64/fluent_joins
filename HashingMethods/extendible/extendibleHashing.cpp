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
//        numberBucketFetch += bucket->getChainCount();
        numberBucketFetch++;
    }

    return bucket;
}

DepthBucket *ExtendibleHashing::getBucket(size_t hash)
{
    DepthBucket *bucket = directory.getBucket(hash);
    if (bucket->size() > 0) {
//        numberBucketFetch += bucket->getChainCount();
        numberBucketFetch++;
    }

    return bucket;
}

vector<Couple> ExtendibleHashing::fetchAllCouples()
{
    vector<Couple> couples;
    vector<DepthBucket *> buckets = directory.fetchBuckets();
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

vector<Couple> ExtendibleHashing::getCouples(size_t keyHash, int keyHashSize, int position, size_t keyHash2, int keyHashSize2, int position2)
{
    DepthBucket *bucket;
    vector<Couple> couples;
    vector<size_t> hashes;
    getHashes(keyHash, keyHashSize, position, keyHash2, keyHashSize2, position2, hashes);
    for (int i = 0; i < hashes.size(); i++) {
        bucket = getBucket(hashes[i]);
        vector<Couple> values = bucket->getAllValues();
        couples.insert(couples.end(), values.begin(), values.end());
    }
    return couples;
}

void ExtendibleHashing::reset()
{
    directory.reset();
}

void ExtendibleHashing::loadBuckets(size_t keyHash, int keyHashSize, int position)
{
    if (position >= 0) {
        vector<size_t> hashes;
        getHashes(keyHash, keyHashSize, position, 0, 0, -1, hashes);
        for (int i = 0; i < hashes.size(); i++) {
            directory.loadBucket(hashes[i]);
        }
    } else {
        directory.loadBuckets();
    }
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

    vector<DepthBucket *> buckets = directory.getBuckets();
    DepthBucket* bucket;
    int maxChainLength = 0;
    int numberOverflowBuckets = 0;
    int numberLongChain = 0;
    int numberChain = 0;
    for (int i = 0; i < buckets.size(); i++) {
        bucket = buckets[i];
        int chainCount = bucket->getChainCount() - 1;

        if (chainCount > 0)
            numberOverflowBuckets += 1;
        if (chainCount == 1)
            numberChain += 1;
        if (chainCount > maxChainLength)
            maxChainLength = chainCount;
        if (chainCount > 1)
            numberLongChain += 1;
    }

    cout << "number overflow buckets : " << numberOverflowBuckets << endl;
    cout << "number chains : " << numberChain << endl;
    cout << "max chain length : " << maxChainLength << endl;
    cout << "number long chains : " << numberLongChain << endl;
    cout << "keys repartition : [";
    for (int i = 0; i < keysRepartition.size(); i++)
        cout << keysRepartition[i] << ", ";
    cout << "]" << endl;
    cout << "load factor : " << (double) numberItems / (BucketFactory<DepthBucket>::getInstance()->getNumberBuckets() * Bucket::BUCKET_SIZE) << "\n" << endl;

    BucketFactory<DepthBucket>::getInstance()->writeAll(buckets, bucketPath);
}

bool ExtendibleHashing::addBHF() {
    double maxNeededBHFRatio = 0.0;
    int maxNeededBHFRatioIndex = 0;
    int totalKeysRepartition = 0;

    for (int i = 0; i < keysRepartition.size(); i++) {
        if (keysRepartition[i] > 0) {
            int neededNumberBuckets = (double) histograms[i].size() / Bucket::BUCKET_SIZE;
            double neededBHFRatio = ((double) log(neededNumberBuckets) / log(2) - keysRepartition[i]);
            cout << neededBHFRatio << endl;
            if (neededBHFRatio > maxNeededBHFRatio) {
                maxNeededBHFRatio = neededBHFRatio;
                maxNeededBHFRatioIndex = i;
            }
            totalKeysRepartition += keysRepartition[i];
        }
    }

    if (maxNeededBHFRatio <= 0.01 or totalKeysRepartition > directory.getGlobalDepth()) {
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
