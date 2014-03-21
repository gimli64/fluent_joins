#include "linearHashing.h"

const double LinearHashing::SPLIT_RATIO = 0.75;

LinearHashing::LinearHashing()
    :level(0), nextSplitIndex(0), initialNumberBuckets(1), bucketCapacity(ChainedBucket::BUCKET_SIZE), buckets()
{
    buckets.push_back(ChainedBucket(this));
}

string* LinearHashing::getValue(size_t key, string value)
{
    return getBucket(key).getValue(value);
}

void LinearHashing::putValue(size_t key, string value)
{
    getBucket(key).putValue(value);
    numberItems++;
    if (getRatio() > SPLIT_RATIO) {
        split();
    }
}

ChainedBucket &LinearHashing::getBucket(size_t key)
{
    int bucketIndex = key & ((1 << level) - 1);
    if (bucketIndex < nextSplitIndex)
        bucketIndex = key & ((1 << level + 1) - 1);

    return buckets.at(bucketIndex);
}

double LinearHashing::getRatio()
{
    return ((double) numberItems) / (bucketCapacity * numberBuckets);
}

void LinearHashing::incrementSplitIndex()
{
    nextSplitIndex++;
    if (nextSplitIndex == initialNumberBuckets) {
        nextSplitIndex = 0;
        level += 1;
        initialNumberBuckets = buckets.size();
    }
}

void LinearHashing::split()
{
    ChainedBucket& bucketToSplit = buckets.at(nextSplitIndex);
    vector<string> values = bucketToSplit.getAllValues();
    numberBuckets -= bucketToSplit.getChainCount();
    numberItems -= values.size();

    buckets.at(nextSplitIndex) = ChainedBucket(this);
    buckets.push_back(ChainedBucket(this));
    incrementSplitIndex();

    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        put(*it);
    }
}

string LinearHashing::className() const
{
    return "LinearHashing ";
}

std::ostream& LinearHashing::dump(std::ostream& strm) const
{
    const void * address = static_cast<const void*>(this);
    stringstream ss;
    ss << address;
    ostream& output = strm;
    output << className() + ss.str() + " : \n";
    for(int i = 0; i < buckets.size(); i++) {
        output << "#### " << buckets.at(i);
        if (i < buckets.size() - 1)
            output << "\n";
    }
    return output;
}

ostream& operator<<(ostream& strm, const LinearHashing& dir)
{
    return dir.dump(strm);
}
