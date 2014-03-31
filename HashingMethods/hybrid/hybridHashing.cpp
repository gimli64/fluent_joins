#include "hybridHashing.h"

const double HybridHashing::SPLIT_RATIO = 0.75;

HybridHashing::HybridHashing()
    :level(32), mask(0), nextSplitIndex(0), initialNumberDirectories(1), bucketCapacity(DepthBucket::BUCKET_SIZE),
      dirCapa(HybridDirectory::CAPA), numberDirEntries(0)
{
    factory = BucketFactory<DepthBucket>::getInstance();
    directories.push_back(HybridDirectory(this));
}

string HybridHashing::getValue(size_t hash, string key)
{
    try {
        return getHybridDirectory(hash).getValue(hash, key);
    } catch (string &e) {
        throw e;
    }
}

void HybridHashing::putCouple(size_t hash, Couple couple)
{
    getHybridDirectory(hash).putCouple(hash, couple);
    numberItems++;
    if (getRatio() > SPLIT_RATIO) {
        split();
    }
}

HybridDirectory &HybridHashing::getHybridDirectory(size_t hash)
{
    hash = getLeftMostBits(hash);
    int pageIndex = hash & mask;
    if (pageIndex < nextSplitIndex)
        pageIndex = hash & (mask | (1 << (31 - (level - 1))));

    return directories.at(pageIndex);
}

int HybridHashing::getLeftMostBits(size_t hash)
{
    unsigned int newKey = 0;
    for (int i = 31; i >= level-1; i--) {
        newKey |= ((unsigned int)(hash & (1 << 31)) >> i);
        hash <<=  1;
    }
    return newKey;
}

void HybridHashing::incrementSplitIndex()
{
    nextSplitIndex++;
    if (nextSplitIndex == initialNumberDirectories) {  // we start a new round
        nextSplitIndex = 0;
        level -= 1;
        mask |= (1 << (31 - level));
        initialNumberDirectories = directories.size();
    }
}

double HybridHashing::getRatio()
{
    return ((double) numberItems * numberDirEntries) / (bucketCapacity * factory->getNumberBuckets() * dirCapa * directories.size());
}

void HybridHashing::split()
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

int HybridHashing::getNumberDirEntries()
{
    return numberDirEntries;
}

void HybridHashing::setNumberDirEntries(int number)
{
    numberDirEntries = number;
}

string HybridHashing::className() const
{
    return "HybridHashing ";
}

std::ostream& HybridHashing::dump(std::ostream& strm) const
{
    const void * address = static_cast<const void*>(this);
    stringstream ss;
    ss << address;
    ostream& output = strm;
    output << className() + ss.str() + " : \n";
    for(int i = 0; i < directories.size(); i++) {
        output << "\n## " << directories.at(i);
        if (i < directories.size() - 1)
            output << "\n";
    }
    return output;
}

ostream& operator<<(ostream& strm, const HybridHashing& dir)
{
    return dir.dump(strm);
}

double HybridHashing::getAverageSize()
{
    int sizes = 0;
    for(int i = 0; i < directories.size(); i++) {
        sizes += directories.at(i).getSize();
    }

    return (double) sizes / directories.size();
}
