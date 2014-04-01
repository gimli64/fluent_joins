#include "chainedBucket.h"

ChainedBucket::ChainedBucket()
    :nextBucket(0), Bucket()
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
        if (nextBucketName == "") {
            throw e;
        } else {
            try {
                ChainedBucket *bucket = BucketFactory<ChainedBucket>::getInstance()->readBucket(bucketPath + nextBucketName);
                result = bucket->getValue(key);
                delete bucket;
            } catch (string &e) {
                throw e;
//                delete bucket;
            }
        }
    }
    return result;
}

void ChainedBucket::putCouple(Couple couple) {
    if (this->isFull()) {
        if (!nextBucket) {
            nextBucket = BucketFactory<ChainedBucket>::getInstance()->newBucket();
            nextBucket->bucketPath = bucketPath;
            nextBucketName = nextBucket->name;
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

vector<ChainedBucket *> ChainedBucket::getChain() {
    vector<ChainedBucket *> chain;
    chain.push_back(this);
    if (nextBucket) {
        vector<ChainedBucket *> nextChain = nextBucket->getChain();
        chain.insert(chain.end(), nextChain.begin(), nextChain.end());
    }
    return chain;
}

vector<Couple> ChainedBucket::getAllValues() {
    vector<Couple> values = Bucket::getAllValues();
    if (nextBucket) {
        vector<Couple> nextBucketValues = nextBucket->getAllValues();
        values.insert(values.end(), nextBucketValues.begin(), nextBucketValues.end());
    }
    return values;
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
