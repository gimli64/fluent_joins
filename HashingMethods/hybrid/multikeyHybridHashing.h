#ifndef MULTIKEYHYBRIDHASHING_H
#define MULTIKEYHYBRIDHASHING_H
#include "common/hashing/multikeyHashTable.h"
#include "common/directory/hybridDirectory.h"
#include <boost/serialization/string.hpp>

using namespace std;

class MultikeyHybridHashing : public MultikeyHashTable
{
public:
    MultikeyHybridHashing(string name = "", vector<int> keysRepartition = vector<int>());
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

    virtual Bucket *fetchBucket(size_t hash);

    HybridDirectory& getHybridDirectory(size_t hash);
    int getLeftMostBits(size_t hash);
    void incrementSplitIndex();
    double getRatio();
    void split();

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<MultikeyHashTable>(*this);
        ar & level;
        ar & mask;
        ar & nextSplitIndex;
        ar & directories;
    }
};

ostream& operator<<(ostream&, const MultikeyHybridHashing&);

#endif // HYBRIDHASHING_H
