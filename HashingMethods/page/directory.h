#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "page.h"
#include "bucket.h"
#include "bucketFactory.h"
#include "hashing/hashTable.h"

#include <iostream>
#include <algorithm>
#include <vector>
#include <sstream>

using namespace std;
using namespace boost;

class Directory: public Page
{
public:
    Directory();
    Directory(HashTable *hasher);

    vector<string> getValue(size_t hash, string key);
    void putCouple(size_t hash, Couple couple);

    vector<Bucket *> getBuckets();

    void reset();

    virtual Page *getBucket(size_t hash);
    virtual bool isBucket();

private:
    vector<Page*> pages;
    BucketFactory<Bucket> *factory;
    HashTable *hasher;

    void split(Bucket *bucket);
    void doubleSize();
};

#endif // DIRECTORY_H
