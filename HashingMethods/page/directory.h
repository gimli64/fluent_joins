#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "bucket.h"
#include "bucketFactory.h"
#include "hashing/hashTable.h"
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

    Bucket *getBucket(size_t hash);
    vector<Bucket *> getBuckets();

    Bucket *fetchBucket(size_t hash);
    vector<Bucket *> fetchBuckets();

    void reset();

private:
    int globalDepth;
    string bucketPath;
    vector<string> bucketNames;
    vector<Bucket*> buckets;
    BucketFactory<Bucket> *factory;
    HashTable *hasher;

    void split(Bucket *bucket);
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

#endif // DIRECTORY_H
