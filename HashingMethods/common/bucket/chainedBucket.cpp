#include "chainedBucket.h"

ChainedBucket::ChainedBucket()
    :nextBucket(0)
{
}

ChainedBucket::ChainedBucket(HashingMethod hasher)
    :nextBucket(0), Bucket(hasher)
{
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
