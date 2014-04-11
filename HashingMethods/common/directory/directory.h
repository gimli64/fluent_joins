#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "common/bucket/depthBucket.h"
#include "common/bucket/bucketFactory.h"
#include "common/hashing/hashingMethod.h"
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
    Directory(HashingMethod *hasher);

    void init();
    vector<string> getValue(size_t hash, string key);
    void putCouple(size_t hash, Couple couple);
    int getGlobalDepth();
    int getSize();

    vector<DepthBucket*> &getBuckets();
    void clearBuckets();

    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

protected:
    BucketFactory<DepthBucket> *factory;
    HashingMethod *hasher;
    vector<DepthBucket*> buckets;
    virtual void doubleSize();
    virtual void split(DepthBucket *bucket);

private:
    int globalDepth;
    vector<string> bucketNames;
    string bucketPath;

    DepthBucket *getBucket(size_t hash);
    DepthBucket *getBucketFromName(size_t hash);

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
