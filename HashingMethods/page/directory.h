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

//    Directory();
    Directory(HashTable *hasher = 0);

    vector<Bucket *> getBuckets();
    size_t pageIndex(size_t hash);
    int getLevel();
    void setLevel(int level);

    vector<string> getValue(size_t hash, string key);
    void putCouple(size_t hash, Couple couple);

    Page *getPage(size_t hash);
    Page *getBucket(size_t hash);

    int getGlobalDepth();

    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

private:
    int level;
    vector<Page*> pages;
    Directory *parent;

    BucketFactory<Bucket> *factory;
    HashTable *hasher;

    void split(Bucket *bucket);
    void doubleSize();
    void splitInnerNode(size_t hash);
    void insertNewLevel(size_t hash);
};

#endif // DIRECTORY_H
