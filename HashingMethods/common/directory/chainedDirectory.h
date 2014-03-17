#ifndef CHAINEDDIRECTORY_H
#define CHAINEDDIRECTORY_H
#include "directory.h"

class ChainedDirectory : public Directory
{
public:
    static const int MAX_DOUBLING = 5;

    ChainedDirectory(HashingMethod& hasher);
    ~ChainedDirectory();
    string* getValue(size_t key, string value);
    void putValue(size_t key, string value);
    int getNumberBuckets();
    int getChainCount();
    vector<string> getAllValues();

    virtual string className() const;
    virtual std::ostream& dump(std::ostream& strm) const;
    int numberDoubling;

private:

    int numberBuckets;
    ChainedDirectory* nextDirectory;
    void split(DepthBucket &bucket);
    bool canBeDoubled();
};

#endif // CHAINEDDIRECTORY_H
