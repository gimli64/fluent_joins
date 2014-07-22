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
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

class Directory: public Page
{
public:
    static const int DEPTH_LIMIT = 10;

    Directory(HashTable *hasher = 0);

    vector<string> getValue(size_t hash, string key);
    void putCouple(size_t hash, Couple couple);

    size_t pageIndex(size_t hash);
    Page *getPage(size_t hash);
    Page *getBucket(size_t hash);
    vector<Bucket *> getBuckets();

    int getGlobalDepth();
    double pageSize() const;

    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

private:
    int level;
    size_t hashPrefix;  // For debugging
    vector<Page*> pages;
    Directory *parent;

    BucketFactory<Bucket> *factory;
    HashTable *hasher;

    void split(Bucket *bucket);
    void splitAllBuckets(size_t childIndexFirstBit);
    void doubleSize();
    void splitInnerNode(size_t hash);
    void insertNewLevel(size_t hash);
};

#endif // DIRECTORY_H
