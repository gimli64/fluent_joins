#ifndef HYBRIDDIRECTORY_H
#define HYBRIDDIRECTORY_H

#include "directory.h"

using namespace std;

class HybridDirectory : public Directory
{
public:
    static const int CAPA = 16;

    HybridDirectory();
    HybridDirectory(HashTable *hasher);

    virtual void putCouple(size_t hash, Couple couple);

    int getNumberBuckets();
    vector<Couple> popAllValues();

    virtual string className() const;

private:
    int numberBuckets;

    virtual size_t getHash(Couple couple);
    virtual void split(DepthBucket* bucket);
    virtual void doubleSize();
    void notifyNumberBuckets(int number);
};

#endif // HybridDirectory_H
