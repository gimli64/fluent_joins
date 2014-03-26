#ifndef CHAINEDDIRECTORY_H
#define CHAINEDDIRECTORY_H

#include "directory.h"
#include "common/hashingMethod.h"

using namespace std;

class ChainedDirectory : public Directory
{
public:
    static const int CAPA = 32;

    ChainedDirectory();
    ChainedDirectory(HashingMethod *hasher);

    int getNumberBuckets();
    int getChainCount();
    vector<Couple> popAllValues();

    virtual string className() const;

private:
    int numberBuckets;
    HashingMethod *hasher;
    virtual void split(DepthBucket* bucket);
    virtual void doubleSize();
    void notifyNumberBuckets(int number);
};

#endif // CHAINEDDIRECTORY_H
