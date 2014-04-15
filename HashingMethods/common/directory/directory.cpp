#include "directory.h"

Directory::Directory()
{
    factory = BucketFactory<DepthBucket>::getInstance();
}

Directory::Directory(HashingMethod *hasher)
    :hasher(hasher), globalDepth(0), buckets()
{
    factory = BucketFactory<DepthBucket>::getInstance();
    bucketPath = hasher->getBucketPath();
    DepthBucket *bucket = factory->newBucket();
    buckets.push_back(bucket);
    bucketNames.push_back(bucket->name);
}

vector<string> Directory::getValue(size_t hash, string key)
{
    DepthBucket *bucket = getBucketFromName(hash);
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
        if (bucket->getLocalDepth() == globalDepth) {
            doubleSize();
        }
        if (bucket->getLocalDepth() < globalDepth) {
            split(bucket);
            bucket = getBucket(hash);
        }
    }
    bucket->putCouple(couple);
}

DepthBucket* Directory::getBucket(size_t hash)
{
    return buckets.at(hash & ((1 << globalDepth) - 1));
}

DepthBucket* Directory::getBucketFromName(size_t hash)
{
    return factory->readBucket(bucketPath + bucketNames.at(hash & ((1 << globalDepth) - 1)));
}


void Directory::doubleSize()
{
    size_t old_size = buckets.size();
    buckets.reserve(2 * old_size);
    bucketNames.reserve(2 * old_size);
    for (size_t i = 0; i < old_size; i++) {
        buckets.push_back(buckets.at(i));
        bucketNames.push_back(bucketNames.at(i));
    }
    globalDepth++;
}

void Directory::split(DepthBucket* bucket)
{
    DepthBucket *newBucket1 = factory->newBucket();
    DepthBucket *newBucket2 = factory->newBucket();
    vector<Couple>& values = bucket->getAllValues();

    for (vector<Couple>::iterator it = values.begin(); it != values.end(); ++it) {
        size_t h = hasher->getComplexHash((*it)) & ((1 << globalDepth) - 1);
        if ((h | (1 << bucket->getLocalDepth())) == h)
            newBucket2->putCouple(*it);
        else
            newBucket1->putCouple(*it);
    }

    vector<int> l;
    for(vector<DepthBucket>::size_type i = 0; i < buckets.size(); i++) {
        if(buckets.at(i) == bucket)
            l.push_back(i);
    }

    for(vector<int>::iterator it = l.begin(); it != l.end(); ++it) {
        if ((*it | (1 << bucket->getLocalDepth())) == *it) {
            buckets.at(*it) = newBucket2;
            bucketNames.at(*it) = newBucket2->name;
        }
        else {
            buckets.at(*it) = newBucket1;
            bucketNames.at(*it) = newBucket1->name;
        }
    }

    newBucket1->setLocalDepth(bucket->getLocalDepth() + 1);
    newBucket2->setLocalDepth(newBucket1->getLocalDepth());
    factory->deleteBucket(bucket);
}

int Directory::getGlobalDepth()
{
    return globalDepth;
}

vector<DepthBucket *> &Directory::getBuckets()
{
    return buckets;
}

void Directory::clearBuckets()
{
    set<DepthBucket*> uniqueBuckets = set<DepthBucket*>(buckets.begin(), buckets.end());
    for(set<DepthBucket*>::iterator it = uniqueBuckets.begin(); it != uniqueBuckets.end(); ++it) {
        delete *it;
    }
    buckets.clear();
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
    output << className() + ss.str() + " : \n";

    DepthBucket* bucket;
    for(int i = 0; i < buckets.size(); i++) {
        bucket = buckets.at(i);
        output << "#### " << *bucket;
        if (i < buckets.size() - 1)
            output << "\n";
    }
    return output;
}

int Directory::getSize()
{
    return buckets.size();
}

ostream& operator<<(ostream& strm, const Directory& dir)
{
    return dir.dump(strm);
}
