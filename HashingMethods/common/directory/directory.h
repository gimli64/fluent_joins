#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <iostream>
#include <vector>
#include "depthbucket.h"
#include "hashingMethod.h"

using std::string;
using std::vector;

class Directory
{
public:
    Directory();
    Directory(HashingMethod& hasher);

    string* getValue(size_t key, string value);
    void putValue(size_t key, string value);
    DepthBucket& getBucket(size_t key);
    vector<DepthBucket>& getBuckets();
    int getGlobalDepth();

protected:
    int globalDepth;
    vector<DepthBucket> buckets;
    HashingMethod hasher;
    void split(DepthBucket& bucket);
    void doubleSize();
};

#endif // DIRECTORY_H
