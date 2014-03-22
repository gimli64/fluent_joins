#include "directory.h"

Directory::Directory()
    :globalDepth(0), buckets()
{}

Directory::Directory(HashingMethod *hasher)
    :globalDepth(0), buckets(), hasher(hasher)
{
    DepthBucket bucket(hasher);
    writeBucket(bucket);
    buckets.push_back(bucket.name);
}

string* Directory::getValue(size_t key, string value)
{
    DepthBucket *bucket = getBucket(key);
    string* result = bucket->getValue(value);
    delete bucket;
    return result;
}

void Directory::putValue(size_t key, string value)
{
    DepthBucket* bucket = getBucket(key);
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
    writeBucket(*bucket);
    delete bucket;
}

DepthBucket* Directory::getBucket(size_t key)
{
    return readBucket(buckets.at(key & ((1 << globalDepth) - 1)));
}

DepthBucket* Directory::readBucket(string bucketFile) const
{
    DepthBucket* bucket = new DepthBucket(hasher);
    {
        ifstream ifs(bucketFile.c_str());
        text_iarchive ia(ifs);
        ia >> *bucket;
    }
    return bucket;
}

void Directory::writeBucket(DepthBucket& bucket)
{
    ofstream ofs(bucket.name.c_str());
    {
        text_oarchive oa(ofs);
        oa << bucket;
    }
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
    DepthBucket newBucket1(hasher);
    DepthBucket newBucket2(hasher);
    vector<string>& values = bucket->getAllValues();

    for (vector<string>::iterator it = values.begin(); it != values.end(); ++it) {
        size_t h = hasher->getHash(*it) & ((1 << globalDepth) - 1);
        if ((h | (1 << bucket->getLocalDepth())) == h)
            newBucket2.putValue(*it);
        else
            newBucket1.putValue(*it);
    }

    vector<int> l;
    for(vector<DepthBucket>::size_type i = 0; i < buckets.size(); i++) {
        if(buckets.at(i) == bucket->name)
            l.push_back(i);
    }

    for(vector<int>::iterator it = l.begin(); it != l.end(); ++it) {
        if ((*it | (1 << bucket->getLocalDepth())) == *it) {
            buckets.at(*it) = newBucket2.name;
        }
        else {
            buckets.at(*it) = newBucket1.name;
        }
    }

    newBucket1.setLocalDepth(bucket->getLocalDepth() + 1);
    newBucket2.setLocalDepth(newBucket1.getLocalDepth());
    writeBucket(newBucket1);
    writeBucket(newBucket2);
    remove(bucket->name.c_str());
    delete bucket;
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
        bucket = readBucket(buckets.at(i));
        output << "#### " << *bucket;
        delete bucket;
        if (i < buckets.size() - 1)
            output << "\n";
    }
    return output;
}

ostream& operator<<(ostream& strm, const Directory& dir)
{
    return dir.dump(strm);
}
