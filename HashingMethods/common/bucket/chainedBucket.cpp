#include "chainedBucket.h"

ChainedBucket::ChainedBucket()
    :nextBucket(0), Bucket()
{
}

ChainedBucket::ChainedBucket(HashingMethod *hasher)
    :nextBucket(0), Bucket(hasher)
{
    if (hasher) {
        notifyNumberBuckets(1);
    }
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

void ChainedBucket::notifyNumberBuckets(int numberBuckets)
{
    hasher->setNumberBuckets(hasher->getNumberBuckets() + numberBuckets);
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
