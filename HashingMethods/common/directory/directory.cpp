#include "directory.h"

Directory::Directory()
    :globalDepth(0), buckets()
{
}

Directory::Directory(HashingMethod *hasher)
    :globalDepth(0), buckets(), hasher(hasher)
{
    buckets.push_back(new DepthBucket(hasher));
}

string* Directory::getValue(size_t key, string value)
{
    return getBucket(key)->getValue(value);
}

void Directory::putValue(size_t key, string value)
{
    DepthBucket* bucket = getBucket(key);
    if (bucket->isFull()) {
        if (bucket->getLocalDepth() == globalDepth) {
            doubleSize();
            bucket = getBucket(key);  // Needed because of buckets reallocation in memory
        }
        if (bucket->getLocalDepth() < globalDepth) {
            split(bucket);
            bucket = getBucket(key);
        }
    }
    bucket->putValue(value);
}

DepthBucket* Directory::getBucket(size_t key)
{
    return buckets.at(key & ((1 << globalDepth) - 1));
}

void Directory::doubleSize()
{
    size_t old_size = buckets.size();
    buckets.reserve(2 * old_size);
    for (size_t i = 0; i < old_size; i++)
        buckets.push_back(buckets.at(i));
    globalDepth++;
}

void Directory::split(DepthBucket* bucket)
{
    DepthBucket* newBucket1 = new DepthBucket(hasher);
    DepthBucket* newBucket2 = new DepthBucket(hasher);
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
        if(buckets.at(i) == bucket)
            l.push_back(i);
    }

    for(vector<int>::iterator it = l.begin(); it != l.end(); ++it) {
        if ((*it | (1 << bucket->getLocalDepth())) == *it)
            buckets.at(*it) = newBucket2;
        else
            buckets.at(*it) = newBucket1;
    }

    newBucket1->setLocalDepth(bucket->getLocalDepth() + 1);
    newBucket2->setLocalDepth(newBucket1->getLocalDepth());
}

vector<DepthBucket*>& Directory::getBuckets()
{
    return buckets;
}

int Directory::getGlobalDepth()
{
    return globalDepth;
}

string Directory::className() const
{
    return "Directory ";
}

std::ostream& Directory::dump(std::ostream& strm) const
{
    const void * address = static_cast<const void*>(this);
    stringstream ss;
    ss << address;
    ostream& output = strm;
    output << className() + ss.str() + " : \n";
    for(int i = 0; i < buckets.size(); i++) {
        output << "#### " << *buckets.at(i);
        if (i < buckets.size() - 1)
            output << "\n";
    }
    return output;
}

std::ostream& operator<<(std::ostream& strm, const Directory& dir)
{
    return dir.dump(strm);
}
