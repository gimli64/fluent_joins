#ifndef LINEARHASHING_H
#define LINEARHASHING_H

#include "common/hashingMethod.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using namespace std;
using namespace boost;
using namespace boost::archive;

class ChainedBucket;

class LinearHashing : public HashingMethod
{
public:
    static LinearHashing *getInstance();
    virtual ostream& dump(ostream& strm) const;

private:
    static const double SPLIT_RATIO;
    int level;
    int nextSplitIndex;
    int initialNumberBuckets;
    int bucketCapacity;
    vector<string> buckets;
    static LinearHashing *instance;

    LinearHashing();
    virtual string getValue(size_t key, string value);
    virtual void putValue(size_t key, string value);
    ChainedBucket *getBucket(size_t key);
    ChainedBucket *readBucket(string bucketFile) const;
    ChainedBucket *createBucket();
    void writeBucket(ChainedBucket *bucket);
    double getRatio();
    void incrementSplitIndex();
    void split();
};

ostream& operator<<(ostream&, const LinearHashing&);

#endif // LINEARHASHING_H
