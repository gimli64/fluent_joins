#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "common/bucket/bucket.h"
#include <vector>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/functional/hash.hpp>
#include <boost/lexical_cast.hpp>
#include <math.h>

using namespace std;
using namespace boost;

class HashTable
{
public:
    vector<int> keysRepartition;
    vector<int> interleaveOrder;
    vector<map<string, int> > histograms;

    HashTable(string name = "", vector<int> keysRepartition = vector<int>(), vector<int> interleaveOrder = vector<int>());

    size_t getHash(string key);
    vector<string> get(string key);
    virtual void put(Couple couple);

    size_t getMultikeyHash(Couple &couple);
    void putMultikey(Couple couple);
    vector<Couple> fetchCouples(size_t keyHash, int keyHashSize, int position, size_t keyHash2 = 0, int keyHashSize2 = 0, int position2 = -1);

    virtual void printState();

    int getNumberBucketFetch();
    void setNumberBucketFetch(int number);

    int getGlobalDepthLimit();
    string getBucketPath();
    string getName();

    virtual void addBHF();

protected:
    string bucketPath;
    string name;
    boost::hash<string> string_hasher;
    int globalDepthLimit;
    int numberBucketFetch;
    int numberItems;

private:
    size_t interleaveHashes(vector<size_t> &hashes, vector<int> keysRepartition);
    void getHashes(size_t keyHash, int keyHashSize, int position, size_t keyHash2, int keyHashSize2, int position2, vector<size_t> &hashes);
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
        ar & globalDepthLimit;
        ar & interleaveOrder;
    }
};

#endif // HASHTABLE_H
