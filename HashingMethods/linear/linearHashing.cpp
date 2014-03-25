#include "linearHashing.h"

const double LinearHashing::SPLIT_RATIO = 0.75;

LinearHashing::LinearHashing()
    :level(0), nextSplitIndex(0), initialNumberBuckets(1),
      bucketCapacity(ChainedBucket::BUCKET_SIZE), buckets(), HashingMethod()
{
    factory = BucketFactory<ChainedBucket>::getInstance();
    ChainedBucket *bucket = factory->createBucket();
    buckets.push_back(bucket->name);
    delete bucket;
}

string LinearHashing::getValue(size_t key, string value)
{
    string result;
    ChainedBucket *bucket = getBucket(key);
    try {
        result = bucket->getValue(value);
    } catch (string &e) {
        delete bucket;
        throw e;
    }

    delete bucket;
    return result;
}

void LinearHashing::putValue(size_t key, string value)
{
    ChainedBucket *bucket = getBucket(key);
    bucket->putValue(value);
    factory->writeBucket(bucket);
    numberItems++;
    if (getRatio() > SPLIT_RATIO) {
        split();
    }
    delete bucket;
}

ChainedBucket *LinearHashing::getBucket(size_t key)
{
    int bucketIndex = key & ((1 << level) - 1);
    if (bucketIndex < nextSplitIndex)
        bucketIndex = key & ((1 << level + 1) - 1);

    return factory->readBucket(buckets.at(bucketIndex));
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
    cout << "SPLIT" << endl;
    ChainedBucket *bucketToSplit = factory->readBucket(buckets.at(nextSplitIndex));
    vector<string> values = bucketToSplit->getAllValues();

    ChainedBucket *newBucket1 = factory->createBucket();
    ChainedBucket *newBucket2 = factory->createBucket();
    buckets.at(nextSplitIndex) = newBucket1->name;
    buckets.push_back(newBucket2->name);

    numberItems -= values.size();
    incrementSplitIndex();
    factory->deleteBucket(bucketToSplit);
    delete newBucket1;
    delete newBucket2;

    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        put(*it);
    }
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
        bucket = factory->readBucket(buckets.at(i));
        output << "#### " << *bucket;
        delete bucket;
        if (i < buckets.size() - 1)
            output << "\n";
    }

    return output;
}

ostream& operator<<(ostream& strm, const LinearHashing& dir)
{
    return dir.dump(strm);
}
