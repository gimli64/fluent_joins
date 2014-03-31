#include "chainedBucket.h"

ChainedBucket::ChainedBucket()
    :nextBucket(0), Bucket()
{
}

vector<string> ChainedBucket::getValue(string key)
{
    try {
        return Bucket::getValue(key);
    } catch(string &e) {
        if (!nextBucket) {
            throw e;
        } else {
            try {
                return nextBucket->getValue(key);
            } catch (string &e) {
                throw e;
            }
        }
    }
}

void ChainedBucket::putCouple(Couple couple) {
    if (this->isFull()) {
        if (!nextBucket) {
            nextBucket = BucketFactory<ChainedBucket>::getInstance()->newBucket();
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
