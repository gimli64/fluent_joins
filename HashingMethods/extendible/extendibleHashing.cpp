#include "extendibleHashing.h"

ExtendibleHashing::ExtendibleHashing(string name, vector<int> keysRepartition)
    :directory(Directory(this)), HashTable(name, keysRepartition)
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

Bucket *ExtendibleHashing::fetchBucket(size_t hash)
{
    Bucket *bucket = directory.getBucketFromName(hash);
    if (bucket->elements.size() > 0)
        numberBucketFetch++;

    return bucket;
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

void ExtendibleHashing::checkStructure()
{
    vector<DepthBucket *>::iterator bucket_it;
    vector<DepthBucket *> buckets = directory.getBucketsFromName();
    DepthBucket *bucket;
    maxChainLength = 0;
    numberOverflowBuckets = 0;
    numberLongChain = 0;
    numberChain = 0;

    for (bucket_it = buckets.begin(); bucket_it != buckets.end(); ++bucket_it) {
        bucket = (*bucket_it);

        int chainCount = 0;
        while (bucket->hasNext()) {
            bucket = bucket->next();
            numberOverflowBuckets++;
            chainCount++;
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
    }
    double loadFactor = numberItems / (BucketFactory<DepthBucket>::getInstance()->getNumberBuckets() * Bucket::BUCKET_SIZE);

    cout << "global depth : " << directory.getGlobalDepth() << endl;
    cout << "number buckets : " << numberBucketFetch << endl;
    cout << "number inserted values : " << numberItems << endl;
    cout << "number overflow buckets : " << numberOverflowBuckets << endl;
    cout << "max chain length : " << maxChainLength << endl;
    cout << "number long chains : " << numberLongChain << endl;
    cout << "number chains : " << numberChain << endl;
    cout << "load factor : " << loadFactor << "\n" << endl;
}

void ExtendibleHashing::addBHF() {
    double maxNeededBHFRatio = 0.0;
    double minDistanceToUniform = 1.0;
    int maxNeededBHFRatioIndex = 0;
    int minDistanceToUniformIndex = 0;

    for (int i = 0; i < keysRepartition.size(); i++) {
        if (keysRepartition[i] > 0) {
            double totalDistance = 0.0;
            map<string, int>::iterator it;
            for (it = histograms[i].begin(); it != histograms[i].end(); ++it) {
                totalDistance += abs(((double) it->second / numberItems) - (1.0 / histograms[i].size()));
            }
            if (totalDistance < minDistanceToUniform) {
                minDistanceToUniform = totalDistance;
                minDistanceToUniformIndex = i;
            }

            int neededNumberBuckets = (int) round((double) histograms[i].size() / Bucket::BUCKET_SIZE);
            int neededBHF = 1;
            while (neededNumberBuckets > 1) {
                neededNumberBuckets >>= 1;
                neededBHF += 1;
            }
            double neededBHFRatio = ((double) neededBHF - keysRepartition[i]) * (1 - totalDistance);
            if (neededBHFRatio > maxNeededBHFRatio) {
                maxNeededBHFRatio = neededBHFRatio;
                maxNeededBHFRatioIndex = i;
            }
        }
    }

    globalDepthLimit += 1;
    if (maxNeededBHFRatio == 0.0) {
        keysRepartition[minDistanceToUniformIndex] += 1;
        cout << "Adding BHF on column " << minDistanceToUniformIndex << ", global depth limit : " << globalDepthLimit << endl;
    }
    else {
        keysRepartition[maxNeededBHFRatioIndex] += 1;
        cout << "Adding BHF on column " << maxNeededBHFRatioIndex << ", global depth limit : " << globalDepthLimit << endl;
    }
}

ostream& operator<<(ostream& strm, const ExtendibleHashing& hash)
{
    return hash.dump(strm);
}
