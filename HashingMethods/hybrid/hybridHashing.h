#ifndef HYBRIDHASHING_H
#define HYBRIDHASHING_H
#include "common/hashingMethod.h"
#include "common/directory/hybridDirectory.h"

using std::cout;
using std::endl;

class HybridHashing : public HashingMethod
{
public:
    HybridHashing();
    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

    vector<DepthBucket *> getBuckets();
    void clearBuckets();

    virtual int getNumberDirEntries();
    virtual void setNumberDirEntries(int number);
    double getAverageSize();

private:
    static const double SPLIT_RATIO;
    int level;
    int mask;
    int nextSplitIndex;
    int initialNumberDirectories;
    int bucketCapacity;
    int dirCapa;
    int numberDirEntries;
    vector<HybridDirectory> directories;
    BucketFactory<DepthBucket> *factory;

    virtual vector<string> getValue(size_t hash, string key);
    virtual void putCouple(size_t hash, Couple couple);

    HybridDirectory& getHybridDirectory(size_t hash);
    int getLeftMostBits(size_t hash);
    void incrementSplitIndex();
    double getRatio();
    void split();
};

ostream& operator<<(ostream&, const HybridHashing&);

#endif // HYBRIDHASHING_H
