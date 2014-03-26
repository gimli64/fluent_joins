#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "common/bucket/depthBucket.h"
#include "common/bucket/bucketFactory.h"
#include "common/hashingMethod.h"

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

    void init();
    string getValue(size_t hash, string key);
    void putCouple(size_t hash, Couple couple);
    int getGlobalDepth();
    int getSize();

    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

protected:
    int globalDepth;
    BucketFactory<DepthBucket>* factory;
    vector<string> buckets;

    DepthBucket *getBucket(size_t hash);
    virtual void doubleSize();
    virtual void split(DepthBucket *bucket);
};

ostream& operator<<(ostream&, const Directory&);

#endif // DIRECTORY_H
