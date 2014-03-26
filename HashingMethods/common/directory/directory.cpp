#include "directory.h"

Directory::Directory()
    :globalDepth(0), buckets()
{
    factory = BucketFactory<DepthBucket>::getInstance();
    DepthBucket *bucket = factory->createBucket();
    buckets.push_back(bucket->name);
    delete bucket;
}

string Directory::getValue(size_t key, string value)
{
    string result;
    DepthBucket *bucket = getBucket(key);
    try {
        result = bucket->getValue(value);
    } catch (string &e) {
        delete bucket;
        throw e;
    }

    delete bucket;
    return result;
}

void Directory::putValue(size_t key, string value)
{
    DepthBucket *bucket = getBucket(key);
    if (bucket->isFull()) {
        if (bucket->getLocalDepth() == globalDepth) {
            doubleSize();
        }
        if (bucket->getLocalDepth() < globalDepth) {
            split(bucket);
            bucket = getBucket(key);
        }
    }
    bucket->putValue(value);
    factory->writeBucket(bucket);
    delete bucket;
}

DepthBucket* Directory::getBucket(size_t key)
{
    return factory->readBucket(buckets.at(key & ((1 << globalDepth) - 1)));
}

void Directory::doubleSize()
{
    size_t old_size = buckets.size();
    buckets.reserve(2 * old_size);
    for (size_t i = 0; i < old_size; i++) {
        buckets.push_back(buckets.at(i));
    }
    globalDepth++;
}

void Directory::split(DepthBucket* bucket)
{
    DepthBucket *newBucket1 = factory->createBucket();
    DepthBucket *newBucket2 = factory->createBucket();
    HashingMethod *hasher = HashingMethod::getInstance();
    vector<string>& values = bucket->getAllValues();

    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        size_t h = hasher->getHash(*it) & ((1 << globalDepth) - 1);
        if ((h | (1 << bucket->getLocalDepth())) == h)
            newBucket2->putValue(*it);
        else
            newBucket1->putValue(*it);
    }

    vector<int> l;
    for(vector<DepthBucket>::size_type i = 0; i < buckets.size(); i++) {
        if(buckets.at(i) == bucket->name)
            l.push_back(i);
    }

    for(vector<int>::iterator it = l.begin(); it != l.end(); ++it) {
        if ((*it | (1 << bucket->getLocalDepth())) == *it) {
            buckets.at(*it) = newBucket2->name;
        }
        else {
            buckets.at(*it) = newBucket1->name;
        }
    }

    newBucket1->setLocalDepth(bucket->getLocalDepth() + 1);
    newBucket2->setLocalDepth(newBucket1->getLocalDepth());
    factory->writeBucket(newBucket1);
    factory->writeBucket(newBucket2);

    factory->deleteBucket(bucket);
    delete newBucket1;
    delete newBucket2;
}

int Directory::getGlobalDepth()
{
    return globalDepth;
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
        bucket = factory->readBucket(buckets.at(i));
        output << "#### " << *bucket;
        delete bucket;
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
