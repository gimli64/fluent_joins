#ifndef CHAINEDDIRECTORY_H
#define CHAINEDDIRECTORY_H

#include "directory.h"
#include "common/hashingMethod.h"

class ChainedDirectory : public Directory
{
public:
    static const int MAX_DOUBLING = 7;

    ChainedDirectory();
    string getValue(size_t key, string value);
    void putValue(size_t key, string value);
    int getNumberBuckets();
    int getChainCount();
    vector<string> getAllValues();

    virtual string className() const;
    virtual std::ostream& dump(std::ostream& strm) const;

private:
    int numberDoubling;
    int numberBuckets;
    ChainedDirectory* nextDirectory;
    void split(DepthBucket* bucket);
    bool canBeDoubled();
    void notifyNumberBuckets(int number);
};

#endif // CHAINEDDIRECTORY_H
