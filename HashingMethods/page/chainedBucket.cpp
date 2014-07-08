#include "chainedBucket.h"

ChainedBucket::ChainedBucket(string name)
    :nextBucket(0), Bucket(name)
{
}

ChainedBucket::~ChainedBucket()
{
    delete nextBucket;
}

vector<string> ChainedBucket::getValue(string key)
{
    vector<string> result;
    try {
        result = Bucket::getValue(key);
    } catch(string &e) {
        if (!hasNext()) {
            throw e;
        } else {
            ChainedBucket *bucket;
            try {
                bucket = BucketFactory<ChainedBucket>::getInstance()->readBucket(bucketPath + nextBucketName);
                result = bucket->getValue(key);
                delete bucket;
            } catch (string &e) {
                throw e;
                delete bucket;
            }
        }
    }
    return result;
}

vector<Couple> ChainedBucket::getAllValues()
{
    vector<Couple> values = Bucket::getAllValues();
    if (hasNext()) {
        ChainedBucket *bucket = BucketFactory<ChainedBucket>::getInstance()->readBucket(bucketPath + nextBucketName);
        vector<Couple> nextValues = bucket->getAllValues();
        values.insert(values.end(), nextValues.begin(), nextValues.end());
        delete bucket;
    }

    return values;
}

void ChainedBucket::putCouple(Couple couple) {
    if (this->isFull()) {
        if (!nextBucket) {
            nextBucket = BucketFactory<ChainedBucket>::getInstance()->newBucket();
            nextBucketName = nextBucket->name;
            nextBucket->setBucketPath(bucketPath);
        }
        nextBucket->putCouple(couple);
    } else {
        Bucket::putCouple(couple);
    }
}

int ChainedBucket::getChainCount() {
    if (nextBucket) {
        return 1 + nextBucket->getChainCount();
    } else {
        return 1;
    }
}

void ChainedBucket::setBucketPath(string path)
{
    bucketPath = path;
}

bool ChainedBucket::hasNext()
{
    return nextBucketName != "";
}

ChainedBucket *ChainedBucket::next()
{
    return nextBucket;
}

string ChainedBucket::nextName()
{
    return nextBucketName;
}

string ChainedBucket::className() const
{
    return "ChainedBucket ";
}

ostream& ChainedBucket::dump(ostream &strm) const
{
    ostream& output = Bucket::dump(strm);
    if (nextBucket) {
        output << " --> ";
        return nextBucket->dump(output);
    }
    return output;
}
