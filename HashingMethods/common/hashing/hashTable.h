#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "common/structure/couple.h"
#include "common/bucket/bucket.h"
#include <vector>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/functional/hash.hpp>
#include <math.h>

using namespace std;

class HashTable
{
public:
    vector<int> keysRepartition;

    HashTable(string name = "", vector<int> keysRepartition = vector<int>());

    size_t getHash(string key);
    virtual size_t getMultikeyHash(Couple couple);

    vector<string> get(string key);
    virtual void put(Couple couple);

    void putMultikey(Couple couple);
    vector<Bucket *> fetchBuckets(size_t keyHash, int keyHashSize, int position, size_t keyHash2 = 0, int keyHashSize2 = 0, int position2 = 0);
    vector<Couple> fetchCouples(size_t keyHash, int keyHashSize, int position, size_t keyHash2 = 0, int keyHashSize2 = 0, int position2 = 0);

    virtual int getNumberDirEntries();
    virtual void setNumberDirEntries(int number);
    virtual int getLevel();
    virtual void printState();

    int getNumberBucketFetch();
    void setNumberBucketFetch(int bucketFetch);

    string getBucketPath();
    string getName();
    int getLeftMostBitIndex();

    int numberEmptyBuckets;

protected:
    string bucketPath;
    string name;
    int numberItems;
    boost::hash<string> string_hasher;
    int leftMostBitIndex;
    int numberBucketFetch;
    int totalBucketSize;

private:
    void getHashes(size_t keyHash, int keyHashSize, int position, size_t keyHash2, int keyHashSize2, int position2, vector<size_t> &hashes);
    size_t interleaveHashes(vector<size_t> &hashes);
    virtual Bucket* fetchBucket(size_t hash);

    virtual void putCouple(size_t hash, Couple couple);
    virtual vector<string> getValue(size_t hash, string key);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & bucketPath;
        ar & keysRepartition;
        ar & leftMostBitIndex;
    }
};

#endif // HASHTABLE_H
