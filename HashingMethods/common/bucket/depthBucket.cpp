#include "depthbucket.h"

DepthBucket::DepthBucket(string name)
    :localDepth(0), Bucket(name)
{
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
                bucket = next();
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

int DepthBucket::getChainCount() {
    if (hasNext()) {
        DepthBucket *nextBucket = next();
        int chainCount = 1 + nextBucket->getChainCount();
        delete nextBucket;
        return chainCount;
    } else {
        return 1;
    }
}

void DepthBucket::setBucketPath(string path)
{
    bucketPath = path;
}

void DepthBucket::setNextBucketName(string name)
{
    nextBucketName = name;
}

string DepthBucket::getNextBucketName()
{
    return nextBucketName;
}

void DepthBucket::setNext(DepthBucket * next)
{
    nextBucket = next;
}

bool DepthBucket::hasNext() const
{
    return nextBucketName != "";
}

DepthBucket *DepthBucket::next() const
{
    return nextBucket;
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
    output << " , depth : " << localDepth;
    if (hasNext()) {
        output << " --> ";
        return next()->dump(output);
    }
    return output;
}
