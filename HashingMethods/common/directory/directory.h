#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "common/bucket/depthBucket.h"
#include "common/bucket/bucketFactory.h"
#include "common/hashing/hashTable.h"
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;
using namespace boost;

class Directory
{
public:
    Directory();
    Directory(HashTable *hasher);

    void init();
    vector<string> getValue(size_t hash, string key);
    virtual void putCouple(size_t hash, Couple couple);
    int getGlobalDepth();
    int getSize();

    DepthBucket *getBucket(size_t hash);
    DepthBucket *getBucketFromName(size_t hash);
    vector<DepthBucket *> getBuckets();
    vector<DepthBucket *> getBucketsFromName();
    void clearBuckets();
    void reset();

    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

protected:
    int globalDepth;

    BucketFactory<DepthBucket> *factory;
    vector<DepthBucket*> buckets;
    HashTable *hasher;
    virtual void doubleSize();
    virtual void split(DepthBucket *bucket);

private:
    vector<string> bucketNames;
    string bucketPath;
    map<string, bool> bucketFetched;

    virtual size_t getHash(Couple couple);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & globalDepth;
        ar & bucketPath;
        ar & bucketNames;
    }
};

ostream& operator<<(ostream&, const Directory&);

#endif // DIRECTORY_H
