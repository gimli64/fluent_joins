#include "linearHashing.h"
#include "common/bucket/chainedBucket.h"

const double LinearHashing::SPLIT_RATIO = 0.75;
LinearHashing *LinearHashing::instance = 0;

LinearHashing::LinearHashing()
    :level(0), nextSplitIndex(0), initialNumberBuckets(1),
      bucketCapacity(ChainedBucket::BUCKET_SIZE), buckets(), HashingMethod()
{
}

LinearHashing *LinearHashing::getInstance()
{
    if (instance == 0) {
        instance = new LinearHashing();
        ChainedBucket *bucket = instance->createBucket();
        instance->buckets.push_back(bucket->name);
        delete bucket;
    }
    return instance;
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
    writeBucket(bucket);
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

    return readBucket(buckets.at(bucketIndex));
}

ChainedBucket *LinearHashing::readBucket(string bucketFile) const
{
    ChainedBucket *bucket = new ChainedBucket;
    {
        ifstream ifs(bucketFile.c_str());
        text_iarchive ia(ifs);
        ia >> *bucket;
    }
    return bucket;
}

ChainedBucket *LinearHashing::createBucket()
{
    ChainedBucket *bucket = new ChainedBucket;
    numberBuckets++;
    writeBucket(bucket);
    return bucket;
}

void LinearHashing::writeBucket(ChainedBucket *bucket)
{
    ofstream ofs(bucket->name.c_str());
    {
        text_oarchive oa(ofs);
        oa << *bucket;
    }
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
    ChainedBucket *bucketToSplit = readBucket(buckets.at(nextSplitIndex));
    vector<string> values = bucketToSplit->getAllValues();

    ChainedBucket *newBucket1 = createBucket();
    ChainedBucket *newBucket2 = createBucket();
    buckets.at(nextSplitIndex) = newBucket1->name;
    buckets.push_back(newBucket2->name);
    writeBucket(newBucket1);
    writeBucket(newBucket2);

    numberBuckets -= bucketToSplit->getChainCount();
    numberItems -= values.size();
    incrementSplitIndex();

    remove(bucketToSplit->name.c_str());
    delete bucketToSplit;
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
        bucket = readBucket(buckets.at(i));
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
