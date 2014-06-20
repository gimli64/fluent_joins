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

    vector<string> getValue(size_t hash, string key);
    void putCouple(size_t hash, Couple couple);

    int getGlobalDepth();

    DepthBucket *getBucket(size_t hash);
    vector<DepthBucket *> getBuckets();

    DepthBucket *fetchBucket(size_t hash);
    vector<DepthBucket *> fetchBuckets();

    void reset();
    void loadBuckets();
    void loadBucket(size_t hash);

    string className() const;
    ostream& dump(ostream& strm) const;

private:
    int globalDepth;
    string bucketPath;
    vector<string> bucketNames;
    vector<DepthBucket*> buckets;
    BucketFactory<DepthBucket> *factory;
    HashTable *hasher;

    void split(DepthBucket *bucket);
    void doubleSize();

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
