#ifndef HYBRIDHASHING_H
#define HYBRIDHASHING_H
#include "hashingMethod.h"
#include "chainedDirectory.h"
#include "depthBucket.h"

using std::cout;
using std::endl;

class HybridHashing : public HashingMethod
{
public:
    HybridHashing();
    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

private:
    static const double SPLIT_RATIO;
    int level;
    int mask;
    int nextSplitIndex;
    int initialNumberDirectories;
    int bucketCapacity;
    vector<ChainedDirectory> directories;

    virtual string* getValue(size_t key, string value);
    virtual void putValue(size_t key, string value);
    ChainedDirectory& getChainedDirectory(size_t key);
    int getLeftMostBits(size_t key);
    void incrementSplitIndex();
    double getRatio();
    void split();
};

ostream& operator<<(ostream&, const HybridHashing&);

#endif // HYBRIDHASHING_H
