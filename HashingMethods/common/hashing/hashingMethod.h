#ifndef HASHINGMETHOD_H
#define HASHINGMETHOD_H

#include "common/structure/couple.h"
#include "common/bucket/bucket.h"
#include <vector>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <city.h>
#include <math.h>
#include "common/hashing/MurmurHash3.h"

using namespace std;

class HashingMethod
{
public:
    HashingMethod(string name = "", vector<int> keysRepartition = vector<int>(), int relationSize = 0);

    // Common dictionnary behaviour methods
    size_t getHash(string key);
    vector<string> get(string key);
    void put(Couple couple);

    // Multikey extension methods
    vector<Bucket *> fetchBuckets(size_t keyHash, int position);
    size_t getComplexHash(Couple couple);
    void insert(Couple couple);

    virtual int getNumberDirEntries();
    virtual void setNumberDirEntries(int number);
    int getNumberBucketFetch();

    string getBucketPath();
    string getName();

    int getRelationSize();

protected:
    string bucketPath;
    string name;
    int numberItems;
    vector<int> keysRepartition;
    const int relationSize;
    int numberBucketFetch;

private:
    void getHashes(size_t keyHash, int position, vector<size_t> &hashes);
    size_t interleaveHashes(vector<size_t> &hashes);

    // Common dictionary behaviour methods
    virtual vector<string> getValue(size_t hash, string key);
    virtual void putCouple(size_t hash, Couple couple);

    virtual Bucket* fetchBucket(size_t hash);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & bucketPath;
        ar & keysRepartition;
    }
};

#endif // HASHINGMETHOD_H
