#include "directory.h"

Directory::Directory()
{
    factory = BucketFactory<DepthBucket>::getInstance();
}

Directory::Directory(HashTable *hasher)
    :hasher(hasher), globalDepth(0)
{
    factory = BucketFactory<DepthBucket>::getInstance();
    bucketPath = hasher->getBucketPath();

    DepthBucket *bucket = factory->newBucket();
    bucket->setBucketPath(bucketPath);
    bucketNames.push_back(bucket->name);
    buckets.push_back(bucket);
}

vector<string> Directory::getValue(size_t hash, string key)
{
    DepthBucket *bucket = getBucket(hash);
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
    DepthBucket *bucket = getBucket(hash);

    if (bucket->isFull()) {
        if (bucket->getLocalDepth() <= hasher->getGlobalDepthLimit()) {
            if (bucket->getLocalDepth() == globalDepth) {
                doubleSize();
            }
            if (bucket->getLocalDepth() < globalDepth) {
                split(bucket);
                bucket = getBucket(hash);
            }
        }
    }

    while (bucket->isFull()) {
        if (bucket->hasNext()) {
            bucket = bucket->getNext();
        } else {
            DepthBucket *nextBucket = factory->newBucket();
            bucket->setNextBucketName(nextBucket->name);
            bucket->setNext(nextBucket);
            nextBucket->setBucketPath(bucketPath);
            bucket = nextBucket;
        }
    }
    bucket->putCouple(couple);
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

void Directory::split(DepthBucket* bucket)
{
    DepthBucket *newBucket1 = factory->newBucket();
    DepthBucket *newBucket2 = factory->newBucket();
    vector<Couple> values = bucket->elements;

    DepthBucket *nextBucket = bucket;
    while (nextBucket->hasNext()) {
        nextBucket = nextBucket->getNext();
        values.insert(values.end(), nextBucket->elements.begin(), nextBucket->elements.end());
    }

    for (vector<Couple>::iterator it = values.begin(); it != values.end(); ++it) {
        size_t h = hasher->getMultikeyHash(*it) & ((1 << globalDepth) - 1);
        if ((h | (1 << bucket->getLocalDepth())) == h)
            newBucket2->putCouple(*it);
        else
            newBucket1->putCouple(*it);
    }

    vector<int> l;
    for(vector<DepthBucket>::size_type i = 0; i < bucketNames.size(); i++) {
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

DepthBucket* Directory::getBucket(size_t hash)
{
    return buckets.at(hash & ((1 << globalDepth) - 1));
}

DepthBucket *Directory::fetchBucket(size_t hash)
{
    return factory->readBucket(bucketPath + bucketNames.at(hash & ((1 << globalDepth) - 1)));
}

vector<DepthBucket *> Directory::getBuckets()
{
    set<DepthBucket *> uniqueBuckets = set<DepthBucket *>(buckets.begin(), buckets.end());
    vector<DepthBucket *> buckets;
    buckets.reserve(uniqueBuckets.size());
    for (set<DepthBucket*>::iterator it = uniqueBuckets.begin(); it != uniqueBuckets.end(); ++it) {
        buckets.push_back(*it);
    }

    return buckets;
}

vector<DepthBucket *> Directory::fetchBuckets()
{
    set<string> uniqueNames = set<string>(bucketNames.begin(), bucketNames.end());
    vector<DepthBucket *> buckets;
    buckets.reserve(uniqueNames.size());
    for (set<string>::iterator it = uniqueNames.begin(); it != uniqueNames.end(); ++it) {
        buckets.push_back(factory->readBucket(bucketPath + *it));
    }

    return buckets;
}

void Directory::reset()
{
    buckets.clear();
    buckets.resize(bucketNames.size(), new DepthBucket());
}

void Directory::loadBuckets()
{
    for (int i = 0; i < bucketNames.size(); i++) {
        buckets[i] = (factory->readBucket(bucketPath + bucketNames[i]));
    }

    DepthBucket *bucket;
    DepthBucket *nextBucket;
    for (vector<DepthBucket *>::iterator it = buckets.begin(); it != buckets.end(); ++it) {
        bucket = *it;

        while (bucket->hasNext()) {
            nextBucket = factory->readBucket(bucketPath + bucket->getNextBucketName());
            bucket->setNext(nextBucket);
            bucket = nextBucket;
        }
    }
}

void Directory::loadBucket(size_t hash)
{
    int bucketIndex = hash & ((1 << globalDepth) - 1);
    string bucketName = bucketNames[bucketIndex];

    DepthBucket *originalBucket = factory->readBucket(bucketPath + bucketName);
    DepthBucket *bucket = originalBucket;
    DepthBucket *nextBucket;
    while (bucket->hasNext()) {
        nextBucket = factory->readBucket(bucketPath + bucket->getNextBucketName());
        bucket->setNext(nextBucket);
        bucket = nextBucket;
    }

    bucket = originalBucket;
    for (int i = 0; i < bucketNames.size(); i++) {
        if (bucketNames[i] == bucketName)
            buckets[i] = bucket;
    }
}

string Directory::className() const
{
    return "Directory ";
}

ostream& Directory::dump(ostream& strm) const
{
    const void * address = static_cast<const void*>(this);
    stringstream ss;
    ss << address;
    ostream& output = strm;
    output << className() + ss.str() + ", globalDepth ";
    output << globalDepth;
    output << " : \n";

    DepthBucket* bucket;
    for(int i = 0; i < buckets.size(); i++) {
        bucket = buckets[i];
        output << "#### " << *bucket;
        if (i < buckets.size() - 1)
            output << "\n";
    }
    return output;
}

ostream& operator<<(ostream& strm, const Directory& dir)
{
    return dir.dump(strm);
}
