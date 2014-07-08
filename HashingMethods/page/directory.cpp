#include "directory.h"

Directory::Directory()
{
    factory = BucketFactory<Bucket>::getInstance();
}

Directory::Directory(HashTable *hasher)
    :hasher(hasher), globalDepth(0)
{
    factory = BucketFactory<Bucket>::getInstance();
    bucketPath = hasher->getBucketPath();

    Bucket *bucket = factory->newBucket();
    bucket->setBucketPath(bucketPath);
    bucketNames.push_back(bucket->name);
    buckets.push_back(bucket);
}

vector<string> Directory::getValue(size_t hash, string key)
{
    Bucket *bucket = getBucket(hash);
    try {
        vector<string> result = bucket->getValue(key);
        delete bucket;
        return result;

    } catch (string &e) {
        delete bucket;
        throw e;
    }
}

void Directory::putCouple(size_t hash, Couple couple)
{
    Bucket *bucket = getBucket(hash);

    if (!bucket->isFull()) {
        bucket->putCouple(couple);
    } else {
        if (bucket->getLocalDepth() == globalDepth) {
            doubleSize();
        }
        if (bucket->getLocalDepth() < globalDepth) {
            split(bucket);
            putCouple(hash, couple);
        }
    }
}

void Directory::doubleSize()
{
    size_t old_size = bucketNames.size();
    bucketNames.reserve(2 * old_size);
    buckets.reserve(2 * old_size);
    for (size_t i = 0; i < old_size; i++) {
        bucketNames.push_back(bucketNames.at(i));
        buckets.push_back(buckets.at(i));
    }
    globalDepth++;
}

void Directory::split(Bucket* bucket)
{
    Bucket *newBucket1 = factory->newBucket();
    Bucket *newBucket2 = factory->newBucket();
    vector<Couple> values = bucket->getAllValues();

    for (vector<Couple>::iterator it = values.begin(); it != values.end(); ++it) {
        size_t h = hasher->getMultikeyHash(*it) & ((1 << globalDepth) - 1);
        if ((h | (1 << bucket->getLocalDepth())) == h)
            newBucket2->putCouple(*it);
        else
            newBucket1->putCouple(*it);
    }

    vector<int> l;
    for(vector<Bucket>::size_type i = 0; i < bucketNames.size(); i++) {
        if(bucketNames.at(i) == bucket->name)
            l.push_back(i);
    }

    for(vector<int>::iterator it = l.begin(); it != l.end(); ++it) {
        if ((*it | (1 << bucket->getLocalDepth())) == *it) {
            bucketNames.at(*it) = newBucket2->name;
            buckets.at(*it) = newBucket2;
        }
        else {
            bucketNames.at(*it) = newBucket1->name;
            buckets.at(*it) = newBucket1;
        }
    }

    newBucket1->setLocalDepth(bucket->getLocalDepth() + 1);
    newBucket1->setBucketPath(bucketPath);
    newBucket2->setLocalDepth(newBucket1->getLocalDepth());
    newBucket2->setBucketPath(bucketPath);
    factory->deleteBucket(bucket);
}

int Directory::getGlobalDepth()
{
    return globalDepth;
}

Bucket *Directory::getBucket(size_t hash)
{
    return buckets.at(hash & ((1 << globalDepth) - 1));
}

Bucket *Directory::fetchBucket(size_t hash)
{
    return factory->readBucket(bucketPath + bucketNames.at(hash & ((1 << globalDepth) - 1)));
}

vector<Bucket *> Directory::getBuckets()
{
    set<Bucket *> uniqueBuckets = set<Bucket *>(buckets.begin(), buckets.end());
    vector<Bucket *> buckets;
    buckets.reserve(uniqueBuckets.size());
    for (set<Bucket*>::iterator it = uniqueBuckets.begin(); it != uniqueBuckets.end(); ++it) {
        buckets.push_back(*it);
    }

    return buckets;
}

vector<Bucket *> Directory::fetchBuckets()
{
    set<string> uniqueNames = set<string>(bucketNames.begin(), bucketNames.end());
    vector<Bucket *> buckets;
    buckets.reserve(uniqueNames.size());
    for (set<string>::iterator it = uniqueNames.begin(); it != uniqueNames.end(); ++it) {
        buckets.push_back(factory->readBucket(bucketPath + *it));
    }

    return buckets;
}
