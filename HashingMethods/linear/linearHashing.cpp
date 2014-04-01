#include "linearHashing.h"

const double LinearHashing::SPLIT_RATIO = 0.75;

LinearHashing::LinearHashing()
    :level(0), nextSplitIndex(0), initialNumberBuckets(1),
      bucketCapacity(ChainedBucket::BUCKET_SIZE), buckets(), bucketNames(), HashingMethod()
{
    factory = BucketFactory<ChainedBucket>::getInstance();
    ChainedBucket *bucket = factory->newBucket();
    bucket->bucketPath = bucketPath;
    buckets.push_back(bucket);
    bucketNames.push_back(bucket->name);
}

vector<string> LinearHashing::getValue(size_t hash, string key)
{
    ChainedBucket *bucket = getBucketFromName(hash);
    try {
        vector<string> result = bucket->getValue(key);
        delete bucket;
        return result;

    } catch (string &e) {
        delete bucket;
        throw e;
    }
}

void LinearHashing::putCouple(size_t hash, Couple couple)
{
    ChainedBucket *bucket = getBucket(hash);
    bucket->putCouple(couple);
    numberItems++;
    if (getRatio() > SPLIT_RATIO) {
        split();
    }
}

ChainedBucket *LinearHashing::getBucket(size_t hash)
{
    int bucketIndex = hash & ((1 << level) - 1);
    if (bucketIndex < nextSplitIndex)
        bucketIndex = hash & ((1 << level + 1) - 1);

    return buckets.at(bucketIndex);
}

ChainedBucket *LinearHashing::getBucketFromName(size_t hash)
{
    int bucketIndex = hash & ((1 << level) - 1);
    if (bucketIndex < nextSplitIndex)
        bucketIndex = hash & ((1 << level + 1) - 1);

    return factory->readBucket(bucketPath + bucketNames.at(bucketIndex));
}

double LinearHashing::getRatio()
{
    return ((double) numberItems) / (bucketCapacity * factory->getNumberBuckets());
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
    ChainedBucket *bucketToSplit = buckets.at(nextSplitIndex);
    vector<Couple> values = bucketToSplit->getAllValues();

    ChainedBucket *newBucket1 = factory->newBucket();
    newBucket1->bucketPath = bucketPath;
    ChainedBucket *newBucket2 = factory->newBucket();
    newBucket2->bucketPath = bucketPath;
    buckets.at(nextSplitIndex) = newBucket1;
    buckets.push_back(newBucket2);
    bucketNames.at(nextSplitIndex) = newBucket1->name;
    bucketNames.push_back(newBucket2->name);

    numberItems -= values.size();
    incrementSplitIndex();
    factory->deleteBucket(bucketToSplit);

    for (vector<Couple>::iterator it = values.begin(); it != values.end(); ++it) {
        put(*it);
    }
}

vector<ChainedBucket*> LinearHashing::getBuckets()
{
    vector<ChainedBucket *> allBuckets;
    for(vector<ChainedBucket*>::iterator it = buckets.begin(); it != buckets.end(); ++it) {
        vector<ChainedBucket *> chain = (*it)->getChain();
        allBuckets.insert(allBuckets.end(), chain.begin(), chain.end());
    }
    return allBuckets;
}

void LinearHashing::clearBuckets()
{
    for(vector<ChainedBucket*>::iterator it = buckets.begin(); it != buckets.end(); ++it) {
        delete *it;
    }
    buckets.clear();
}

std::ostream& LinearHashing::dump(std::ostream& strm) const
{
    const void * address = static_cast<const void*>(this);
    stringstream ss;
    ss << address;
    ostream& output = strm;
    output << "LinearHashing " + ss.str() + " : \n";

    ChainedBucket* bucket;
    for(int i = 0; i < buckets.size(); i++) {
        bucket = buckets.at(i);
        output << "#### " << *bucket;
        if (i < buckets.size() - 1)
            output << "\n";
    }

    return output;
}

ostream& operator<<(ostream& strm, const LinearHashing& dir)
{
    return dir.dump(strm);
}
