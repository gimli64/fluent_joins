#ifndef LINEARHASHING_H
#define LINEARHASHING_H
#include "hashingMethod.h"
#include "chainedBucket.h"

using std::ostream;
using std::cout; using std::endl;

class LinearHashing : public HashingMethod
{
public:
    LinearHashing();
    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

private:
    static const double SPLIT_RATIO;
    int level;
    int nextSplitIndex;
    int initialNumberBuckets;
    int bucketCapacity;
    vector<ChainedBucket> buckets;

    virtual string* getValue(size_t key, string value);
    virtual void putValue(size_t key, string value);
    ChainedBucket& getBucket(size_t key);
    double getRatio();
    void incrementSplitIndex();
    void split();
};

ostream& operator<<(ostream&, const LinearHashing&);

#endif // LINEARHASHING_H
