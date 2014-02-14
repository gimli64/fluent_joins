#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <iostream>
#include "depthbucket.h"

using std::string;

class Directory
{
public:
    Directory();

    string* getValue(int key, string value);
    void putValue(int key, string value);
    DepthBucket getBucket(int key);
    void doubleSize();
    int getSize();

protected:
    int globalDepth;
    void split(DepthBucket bucket);
};

#endif // DIRECTORY_H
