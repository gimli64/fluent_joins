#ifndef HYBRIDDIRECTORY_H
#define HYBRIDDIRECTORY_H

#include "directory.h"
#include "common/hashingMethod.h"

using namespace std;

class HybridDirectory : public Directory
{
public:
    static const int CAPA = 32;

    HybridDirectory();
    HybridDirectory(HashingMethod *hasher);

    int getNumberBuckets();
    int getChainCount();
    vector<Couple> popAllValues();

    virtual string className() const;

private:
    int numberBuckets;

    virtual void split(DepthBucket* bucket);
    virtual void doubleSize();
    void notifyNumberBuckets(int number);
};

#endif // HybridDirectory_H
