#ifndef MULTIKEYHASHTABLE_H
#define MULTIKEYHASHTABLE_H

#include <common/hashing/hashTable.h>
#include <common/structure/couple.h>
#include <common/bucket/bucket.h>

class MultikeyHashTable : public HashTable
{
public:
    MultikeyHashTable(string name = "", vector<int> keysRepartition = vector<int>());

    vector<int> keysRepartition;
    int leftMostBitIndex;

    using HashTable::getHash;
    virtual size_t getHash(Couple couple);

    vector<Bucket *> fetchBuckets(size_t keyHash, int keyHashSize, int position);
    virtual void put(Couple couple);

private:
    void getHashes(size_t keyHash, int keyHashSize, int position, vector<size_t> &hashes);
    size_t interleaveHashes(vector<size_t> &hashes);
    unsigned int MurmurHash2(const void * key, int len, unsigned int seed);

    virtual Bucket* fetchBucket(size_t hash);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<HashTable>(*this);
        ar & keysRepartition;
        ar & leftMostBitIndex;
    }
};

#endif // MULTIKEYHASHTABLE_H
