#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "depthbucket.h"
#include "hashingMethod.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <boost/lexical_cast.hpp>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ostream;
using std::stringstream;
using std::ofstream;
using std::ifstream;
using std::remove;
using boost::lexical_cast;
using boost::archive::text_iarchive;
using boost::archive::text_oarchive;

class Directory
{
public:
    Directory();
    Directory(HashingMethod* hasher);

    string* getValue(size_t key, string value);
    void putValue(size_t key, string value);
    int getGlobalDepth();

    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

protected:
    int globalDepth;
    HashingMethod* hasher;
    vector<string> buckets;

    DepthBucket *getBucket(size_t key);
    DepthBucket *readBucket(string bucketFile) const;
    void writeBucket(DepthBucket &bucket);
    void doubleSize();
    void split(DepthBucket *bucket);
};

ostream& operator<<(ostream&, const Directory&);

#endif // DIRECTORY_H
