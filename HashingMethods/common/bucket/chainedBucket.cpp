#include "chainedBucket.h"

ChainedBucket::ChainedBucket()
    :nextBucket(0)
{
}

ChainedBucket::ChainedBucket(HashingMethod& hasher)
    :nextBucket(0), Bucket(hasher)
{
}

ChainedBucket::~ChainedBucket()
{
    delete nextBucket;
}

string* ChainedBucket::getValue(string value)
{
    string* result = Bucket::getValue(value);
    if (result == (string*) NULL && nextBucket) {
        result = nextBucket->getValue(value);
    }
    return result;
}

void ChainedBucket::putValue(string value) {
    if (this->isFull()) {
        if (!nextBucket) {
            nextBucket = new ChainedBucket(hasher);
        }
        nextBucket->putValue(value);
    } else {
        Bucket::putValue(value);
    }
}

int ChainedBucket::getChainCount() {
    if (nextBucket) {
        return 1 + nextBucket->getChainCount();
    } else {
        return 1;
    }
}

vector<string> ChainedBucket::getAllValues() {
    vector<string> values = Bucket::getAllValues();
    if (nextBucket) {
        vector<string> nextBucketValues = nextBucket->getAllValues();
        values.insert(values.end(), nextBucketValues.begin(), nextBucketValues.end());
    }
    return values;
}
