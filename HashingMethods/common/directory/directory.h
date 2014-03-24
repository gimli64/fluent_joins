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
    Directory(HashingMethod *hasher);

    void init();
    string getValue(size_t key, string value);
    void putValue(size_t key, string value);
    int getGlobalDepth();

    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

protected:
    int globalDepth;
    BucketFactory<DepthBucket>* factory;
    HashingMethod *hasher;
    vector<string> buckets;

    DepthBucket *getBucket(size_t key);
    void doubleSize();
    void split(DepthBucket *bucket);
};

ostream& operator<<(ostream&, const Directory&);

#endif // DIRECTORY_H
