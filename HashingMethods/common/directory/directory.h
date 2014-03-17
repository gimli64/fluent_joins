#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <iostream>
#include <algorithm>
#include <vector>
#include "depthbucket.h"
#include "hashingMethod.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

class Directory
{
public:
    Directory();
    Directory(HashingMethod& hasher);

    string* getValue(size_t key, string value);
    void putValue(size_t key, string value);
    vector<DepthBucket>& getBuckets();
    int getGlobalDepth();

protected:
    int globalDepth;
    HashingMethod hasher;
    vector<DepthBucket> buckets;
    DepthBucket& getBucket(size_t key);
    void doubleSize();
    void split(DepthBucket& bucket);
};

#endif // DIRECTORY_H
