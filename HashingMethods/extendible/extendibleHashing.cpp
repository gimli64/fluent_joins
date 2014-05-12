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
    Bucket *bucket = directory.fetchBucket(hash);
    if (bucket->size() > 0)
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

void ExtendibleHashing::printState()
{
    cout << "global depth : " << directory.getGlobalDepth() << endl;
    BucketFactory<DepthBucket>::getInstance()->printState();
    cout << "load factor : " << (double) numberItems / (BucketFactory<DepthBucket>::getInstance()->getNumberBuckets() * Bucket::BUCKET_SIZE) << "\n" << endl;
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
        cout << "Adding BHF on column " << minDistanceToUniformIndex << endl;
    }
    else {
        keysRepartition[maxNeededBHFRatioIndex] += 1;
        cout << "Adding BHF on column " << maxNeededBHFRatioIndex << endl;
    }
}

ostream& operator<<(ostream& strm, const ExtendibleHashing& hash)
{
    return hash.dump(strm);
}
