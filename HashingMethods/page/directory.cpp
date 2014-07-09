#include "directory.h"

Directory::Directory()
{
    factory = BucketFactory<Bucket>::getInstance();
}

Directory::Directory(HashTable *hasher)
    :hasher(hasher)
{
    factory = BucketFactory<Bucket>::getInstance();
    Bucket *bucket = factory->newBucket();
    pages.push_back(bucket);
}

vector<string> Directory::getValue(size_t hash, string key)
{
    try {
        return ((Bucket*) getBucket(hash))->getValue(key);
    } catch (string &e) {
        throw e;
    }
}

void Directory::putCouple(size_t hash, Couple couple)
{
    Bucket *bucket = (Bucket*) getBucket(hash);
    if (!bucket->isFull()) {
        bucket->putCouple(couple);
    } else {
        if (bucket->getDepth() == depth) {
            doubleSize();
        }
        if (bucket->getDepth() < depth) {
            split(bucket);
            putCouple(hash, couple);
        }
    }
}

void Directory::doubleSize()
{
    size_t old_size = pages.size();
    pages.reserve(2 * old_size);
    for (size_t i = 0; i < old_size; i++) {
        pages.push_back(pages.at(i));
    }
    depth++;
}

void Directory::split(Bucket* bucket)
{
    Bucket *newBucket1 = factory->newBucket();
    Bucket *newBucket2 = factory->newBucket();
    vector<Couple> values = bucket->getAllValues();

    for (vector<Couple>::iterator it = values.begin(); it != values.end(); ++it) {
        size_t h = hasher->getMultikeyHash(*it) & ((1 << depth) - 1);
        if ((h | (1 << bucket->getDepth())) == h)
            newBucket2->putCouple(*it);
        else
            newBucket1->putCouple(*it);
    }

    vector<int> l;
    for(vector<Bucket>::size_type i = 0; i < pages.size(); i++) {
        if(pages.at(i) == bucket)
            l.push_back(i);
    }

    for(vector<int>::iterator it = l.begin(); it != l.end(); ++it) {
        if ((*it | (1 << bucket->getDepth())) == *it) {
            pages.at(*it) = newBucket2;
        }
        else {
            pages.at(*it) = newBucket1;
        }
    }

    newBucket1->setDepth(bucket->getDepth() + 1);
    newBucket2->setDepth(newBucket1->getDepth());
    factory->deleteBucket(bucket);
}

Page *Directory::getBucket(size_t hash)
{
    return pages.at(hash & ((1 << depth) - 1))->getBucket(hash);
}

bool Directory::isBucket()
{
    return false;
}

vector<Bucket *> Directory::getBuckets()
{
//    set<Bucket *> uniqueBuckets = set<Bucket *>(pages.begin(), pages.end());
//    vector<Bucket *> buckets;
//    buckets.reserve(uniqueBuckets.size());
//    for (set<Bucket*>::iterator it = uniqueBuckets.begin(); it != uniqueBuckets.end(); ++it) {
//        buckets.push_back(*it);
//    }

//    return buckets;
}
