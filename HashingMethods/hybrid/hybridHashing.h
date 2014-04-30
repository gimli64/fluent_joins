#ifndef HYBRIDHASHING_H
#define HYBRIDHASHING_H
#include "common/hashing/multikeyHashTable.h"
#include "common/directory/hybridDirectory.h"
#include <boost/serialization/string.hpp>

using namespace std;

class HybridHashing : public HashTable
{
public:
    HybridHashing(string name = "", vector<int> keysRepartition = vector<int>());
    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

    vector<DepthBucket *> getBuckets();
    void clearBuckets();

    virtual int getNumberDirEntries();
    virtual void setNumberDirEntries(int number);
    double getAverageSize();
    virtual int getLevel();
    void reset();

protected:
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

private:
    virtual vector<string> getValue(size_t hash, string key);
    virtual void putCouple(size_t hash, Couple couple);

    virtual HybridDirectory& getHybridDirectory(size_t hash);
    virtual size_t getLeftMostBits(size_t hash);
    virtual void incrementSplitIndex();
    virtual void split();

    double getRatio();

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<HashTable>(*this);
        ar & level;
        ar & mask;
        ar & nextSplitIndex;
        ar & directories;
    }
};

ostream& operator<<(ostream&, const HybridHashing&);

#endif // HYBRIDHASHING_H
