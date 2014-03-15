#ifndef CHAINEDDIRECTORY_H
#define CHAINEDDIRECTORY_H
#include "directory.h"

class ChainedDirectory : public Directory
{
public:
    ChainedDirectory(HashingMethod& hasher);
    ~ChainedDirectory();
    string* getValue(size_t key, string value);
    void putValue(size_t key, string value);
    int getNumberBuckets();
    int getChainCount();
    vector<string> getAllValues();

private:
    static const int MAX_DOUBLING = 5;
    int numberDoubling;
    int numberBuckets;
    ChainedDirectory* nextDirectory;

    void split(DepthBucket &bucket);
    bool canBeDoubled();
};

#endif // CHAINEDDIRECTORY_H
