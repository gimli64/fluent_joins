#include "hybridHashing.h"

const double HybridHashing::SPLIT_RATIO = 0.75;

HybridHashing::HybridHashing()
    :level(32), mask(0), nextSplitIndex(0), initialNumberDirectories(1), bucketCapacity(DepthBucket::BUCKET_SIZE)
{
    directories.push_back(ChainedDirectory(this));
}

string HybridHashing::getValue(size_t key, string value)
{
    try {
        return getChainedDirectory(key).getValue(key, value);
    } catch (string &e) {
        throw e;
    }
}

void HybridHashing::putValue(size_t key, string value)
{
    getChainedDirectory(key).putValue(key, value);
    numberItems++;
    if (getRatio() > SPLIT_RATIO) {
        split();
    }
}

ChainedDirectory &HybridHashing::getChainedDirectory(size_t key)
{
    key = getLeftMostBits(key);
    int pageIndex = key & mask;
    if (pageIndex < nextSplitIndex)
        pageIndex = key & (mask | (1 << (31 - (level - 1))));

    return directories.at(pageIndex);
}

int HybridHashing::getLeftMostBits(size_t key)
{
    unsigned int newKey = 0;
    for (int i = 31; i >= level-1; i--) {
        newKey |= ((unsigned int)(key & (1 << 31)) >> i);
        key <<=  1;
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
    return ((double) numberItems) / (bucketCapacity * numberBuckets);
}

void HybridHashing::split()
{
    ChainedDirectory &directoryToSplit = directories.at(nextSplitIndex);
    vector<string> values = directoryToSplit.getAllValues();
    numberBuckets -= directoryToSplit.getNumberBuckets();
    numberItems -= values.size();

    directories.at(nextSplitIndex) = ChainedDirectory(this);
    directories.push_back(ChainedDirectory(this));
    incrementSplitIndex();

    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it)
        HashingMethod::put(*it);
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
