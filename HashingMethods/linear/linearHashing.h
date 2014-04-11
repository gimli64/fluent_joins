#ifndef LINEARHASHING_H
#define LINEARHASHING_H

#include "common/hashing/hashingMethod.h"
#include "common/bucket/chainedBucket.h"
#include "common/bucket/bucketFactory.h"
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

using namespace std;

class LinearHashing : public HashingMethod
{
public:
    LinearHashing(string name = "", vector<int> keysRepartition = vector<int>(), int relationSize = 0);
    virtual ostream& dump(ostream& strm) const;

    vector<ChainedBucket*> getBuckets();
    void clearBuckets();

private:
    static const double SPLIT_RATIO;
    int level;
    int nextSplitIndex;
    int initialNumberBuckets;
    int bucketCapacity;
    vector<ChainedBucket*> buckets;
    vector<string> bucketNames;
    BucketFactory<ChainedBucket> *factory;

    virtual vector<string> getValue(size_t hash, string key);
    virtual void putCouple(size_t hash, Couple couple);

    ChainedBucket *getBucket(size_t hash);
    ChainedBucket *getBucketFromName(size_t hash);

    double getRatio();
    void incrementSplitIndex();
    void split();

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<HashingMethod>(*this);
        ar & level;
        ar & nextSplitIndex;
        ar & bucketNames;
    }
};

ostream& operator<<(ostream&, const LinearHashing&);

#endif // LINEARHASHING_H
