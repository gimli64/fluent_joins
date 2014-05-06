#include "depthbucket.h"

DepthBucket::DepthBucket(string name)
    :localDepth(0), nextBucket(0), Bucket(name)
{
}

DepthBucket::~DepthBucket()
{
    delete nextBucket;
}

vector<string> DepthBucket::getValue(string key)
{
    vector<string> result;
    try {
        result = Bucket::getValue(key);
    } catch(string &e) {
        if (!hasNext()) {
            throw e;
        } else {
            DepthBucket *bucket;
            try {
                bucket = BucketFactory<DepthBucket>::getInstance()->readBucket(bucketPath + nextBucketName);
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

vector<Couple> DepthBucket::getAllValues()
{
    vector<Couple> values = Bucket::getAllValues();
    if (hasNext()) {
        DepthBucket *bucket = BucketFactory<DepthBucket>::getInstance()->readBucket(bucketPath + nextBucketName);
        vector<Couple> nextValues = bucket->getAllValues();
        values.insert(values.end(), nextValues.begin(), nextValues.end());
        delete bucket;
    }

    return values;
}

void DepthBucket::putCouple(Couple couple) {
    if (this->isFull()) {
        if (!nextBucket) {
            nextBucket = BucketFactory<DepthBucket>::getInstance()->newBucket();
            nextBucketName = nextBucket->name;
            nextBucket->setBucketPath(bucketPath);
        }
        nextBucket->putCouple(couple);
    } else {
        Bucket::putCouple(couple);
    }
}

int DepthBucket::getChainCount() {
    if (nextBucket) {
        return 1 + nextBucket->getChainCount();
    } else {
        return 1;
    }
}

void DepthBucket::setBucketPath(string path)
{
    bucketPath = path;
}

bool DepthBucket::hasNext()
{
    return nextBucketName != "";
}

DepthBucket *DepthBucket::next()
{
    return nextBucket;
}

string DepthBucket::nextName()
{
    return nextBucketName;
}

int DepthBucket::getLocalDepth() {
    return localDepth;
}

void DepthBucket::setLocalDepth(int depth) {
    localDepth = depth;
}

string DepthBucket::className() const
{
    return "DepthBucket ";
}

ostream& DepthBucket::dump(ostream &strm) const
{
    ostream& output = Bucket::dump(strm);
    if (nextBucket) {
        output << " --> ";
        return nextBucket->dump(output);
    }
    return output << " , depth : " << localDepth;
}
