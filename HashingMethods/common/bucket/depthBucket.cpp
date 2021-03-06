#include "depthbucket.h"

DepthBucket::DepthBucket(string name)
    :localDepth(0), Bucket(name), nextBucket(0)
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
            try {
                result = getNext()->getValue(key);
            } catch (string &e) {
                throw e;
            }
        }
    }
    return result;
}

int DepthBucket::size() {
    int size = Bucket::size();
    if (hasNext()) {
        size += getNext()->size();
    }
    return size;
}

vector<Couple> DepthBucket::getAllValues()
{
    vector<Couple> values = Bucket::getAllValues();
    if (hasNext()) {
        vector<Couple> nextValues = getNext()->getAllValues();
        values.insert(values.end(), nextValues.begin(), nextValues.end());
    }

    return values;
}

int DepthBucket::getChainCount() {
    if (hasNext()) {
        int chainCount = 1 + getNext()->getChainCount();
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

void DepthBucket::setNext(DepthBucket * next)
{
    nextBucket = next;
}

bool DepthBucket::hasNext()
{
    return nextBucketName != "";
}

DepthBucket *DepthBucket::getNext()
{
    if (nextBucket == 0)
        nextBucket = BucketFactory<DepthBucket>::getInstance()->readBucket(bucketPath + nextBucketName);
    return nextBucket;
}

int DepthBucket::getLocalDepth()
{
    return localDepth;
}

void DepthBucket::setLocalDepth(int depth)
{
    localDepth = depth;
}
