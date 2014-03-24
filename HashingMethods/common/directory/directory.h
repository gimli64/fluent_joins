#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "common/bucket/depthBucket.h"
#include "common/hashingMethod.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;
using boost::archive::text_iarchive;
using boost::archive::text_oarchive;

class ExtendibleHashing;

class Directory
{
public:
    Directory();

    void init();
    string getValue(size_t key, string value);
    void putValue(size_t key, string value);
    int getGlobalDepth();

    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

protected:
    int globalDepth;
    vector<string> buckets;

    DepthBucket *getBucket(size_t key);
    DepthBucket *readBucket(string bucketFile) const;
    DepthBucket *createBucket();
    void notifyBucket();
    void unNotifyBucket();
    void writeBucket(DepthBucket *bucket);
    void doubleSize();
    void split(DepthBucket *bucket);
};

ostream& operator<<(ostream&, const Directory&);

#endif // DIRECTORY_H
