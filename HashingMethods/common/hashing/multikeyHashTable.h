#ifndef MULTIKEYHASHTABLE_H
#define MULTIKEYHASHTABLE_H

#include <common/hashing/hashTable.h>
#include <common/structure/couple.h>
#include <common/bucket/bucket.h>

class MultikeyHashTable : public HashTable
{
public:
    MultikeyHashTable(string name = "", vector<int> keysRepartition = vector<int>());

    virtual size_t getHash(Couple couple);

    vector<Bucket *> fetchBuckets(size_t keyHash, int position);
    virtual void put(Couple couple);

private:
    vector<int> keysRepartition;

    void getHashes(size_t keyHash, int position, vector<size_t> &hashes);
    size_t interleaveHashes(vector<size_t> &hashes);

    virtual Bucket* fetchBucket(size_t hash);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<HashTable>(*this);
        ar & keysRepartition;
    }
};

#endif // MULTIKEYHASHTABLE_H
