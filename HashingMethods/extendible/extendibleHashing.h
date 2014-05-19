#ifndef EXTENDIBLEHASHING_H
#define EXTENDIBLEHASHING_H
#include "common/hashing/hashTable.h"
#include "common/directory/directory.h"
#include <sstream>

using std::ostream;

class Directory;

class ExtendibleHashing : public HashTable
{
public:
    ExtendibleHashing(string name = "", vector<int> keysRepartition = vector<int>(), vector<int> interleaveOrder = vector<int>());
    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

    void reset();
    virtual void printState();
    virtual bool addBHF();

    vector<Couple> getCouples(size_t keyHash, int keyHashSize, int position, size_t keyHash2 = 0, int keyHashSize2 = 0, int position2 = -1);
    vector<Couple> fetchCouples(size_t keyHash, int keyHashSize, int position, size_t keyHash2 = 0, int keyHashSize2 = 0, int position2 = -1);
    vector<Couple> fetchAllCouples();
    void loadBuckets();

private:
    int maxChainLength;
    int numberOverflowBuckets;
    int numberLongChain;
    int numberChain;

    Directory directory;

    virtual vector<string> getValue(size_t hash, string key);
    virtual void putCouple(size_t hash, Couple couple);

    DepthBucket *getBucket(size_t hash);
    virtual DepthBucket *fetchBucket(size_t hash);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<HashTable>(*this);
        ar & directory;
    }
};

ostream& operator<<(ostream&, const ExtendibleHashing&);

#endif // EXTENDIBLEHASHING_H
