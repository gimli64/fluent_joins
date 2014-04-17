#include "multikeyHybridHashing.h"

const double MultikeyHybridHashing::SPLIT_RATIO = 0.75;

MultikeyHybridHashing::MultikeyHybridHashing(string name, vector<int> keysRepartition)
    :mask(0), nextSplitIndex(0), initialNumberDirectories(1), bucketCapacity(DepthBucket::BUCKET_SIZE),
      dirCapa(HybridDirectory::CAPA), numberDirEntries(0), MultikeyHashTable(name, keysRepartition)
{
    factory = BucketFactory<DepthBucket>::getInstance();
    level = leftMostBitIndex;

    if (name != "")
        directories.push_back(HybridDirectory(this));
}

vector<string> MultikeyHybridHashing::getValue(size_t hash, string key)
{
    try {
        return getHybridDirectory(hash).getValue(hash, key);
    } catch (string &e) {
        throw e;
    }
}

void MultikeyHybridHashing::putCouple(size_t hash, Couple couple)
{
    getHybridDirectory(hash).putCouple(hash, couple);
    numberItems++;
    if (getRatio() > SPLIT_RATIO) {
        split();
    }
}

Bucket *MultikeyHybridHashing::fetchBucket(size_t hash)
{
    numberBucketFetch++;
    return getHybridDirectory(hash).getBucketFromName(hash);
}

HybridDirectory &MultikeyHybridHashing::getHybridDirectory(size_t hash)
{
    hash = getLeftMostBits(hash);
    int pageIndex = hash & mask;
    if (pageIndex < nextSplitIndex) {
        pageIndex = hash & (mask | (1 << (leftMostBitIndex - level)));
    }

    return directories.at(pageIndex);
}

int MultikeyHybridHashing::getLeftMostBits(size_t hash)
{
    size_t newKey = 0;
    size_t bit = 0;
    for (int i = leftMostBitIndex; i >= level; i--) {
        bit = (hash & (1 << i)) >> i;
        newKey |= (bit << (leftMostBitIndex - i));
    }
    return newKey;
}

void MultikeyHybridHashing::incrementSplitIndex()
{
    nextSplitIndex++;
    if (nextSplitIndex == initialNumberDirectories) {  // we start a new round
        nextSplitIndex = 0;
        mask |= (1 << (leftMostBitIndex - level));
        level -= 1;
        initialNumberDirectories = directories.size();
    }
}

double MultikeyHybridHashing::getRatio()
{
    return ((double) numberItems * numberDirEntries) / (bucketCapacity * factory->getNumberBuckets() * dirCapa * directories.size());
}

void MultikeyHybridHashing::split()
{
    HybridDirectory directoryToSplit = directories.at(nextSplitIndex);
    vector<Couple> values = directoryToSplit.popAllValues();
    numberDirEntries -= directoryToSplit.getSize();
    numberItems -= values.size();

    directories.at(nextSplitIndex) = HybridDirectory(this);
    directories.push_back(HybridDirectory(this));
    incrementSplitIndex();

    for (vector<Couple>::iterator it = values.begin(); it != values.end(); ++it)
        put(*it);
}

vector<DepthBucket *> MultikeyHybridHashing::getBuckets()
{
    vector<DepthBucket*> buckets;
    for(int i = 0; i < directories.size(); i++) {
        vector<DepthBucket*> &dir_buckets = directories.at(i).getBuckets();
        buckets.insert(buckets.end(), dir_buckets.begin(), dir_buckets.end());
    }

    return buckets;
}

void MultikeyHybridHashing::clearBuckets()
{
    for(int i = 0; i < directories.size(); i++) {
        directories.at(i).clearBuckets();
    }
}

int MultikeyHybridHashing::getNumberDirEntries()
{
    return numberDirEntries;
}

void MultikeyHybridHashing::setNumberDirEntries(int number)
{
    numberDirEntries = number;
}

string MultikeyHybridHashing::className() const
{
    return "MultikeyHybridHashing ";
}

std::ostream& MultikeyHybridHashing::dump(std::ostream& strm) const
{
    const void * address = static_cast<const void*>(this);
    stringstream ss;
    ss << address;
    ostream& output = strm;
    output << "\n\n" << className() + ss.str() + " : \n";
    for(int i = 0; i < directories.size(); i++) {
        output << "\n## " << directories.at(i);
        if (i < directories.size() - 1)
            output << "\n";
    }
    return output;
}

ostream& operator<<(ostream& strm, const MultikeyHybridHashing& dir)
{
    return dir.dump(strm);
}

double MultikeyHybridHashing::getAverageSize()
{
    int sizes = 0;
    for(int i = 0; i < directories.size(); i++) {
        sizes += directories.at(i).getSize();
    }

    return (double) sizes / directories.size();
}
