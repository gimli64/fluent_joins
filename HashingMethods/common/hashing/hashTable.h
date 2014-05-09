#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "common/bucket/bucket.h"
#include <vector>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/functional/hash.hpp>
//#include <boost/accumulators/accumulators.hpp>
//#include <boost/accumulators/statistics/stats.hpp>
//#include <boost/accumulators/statistics/density.hpp>
#include <boost/lexical_cast.hpp>
#include <math.h>

using namespace std;
using namespace boost;
//using namespace boost::accumulators;

class HashTable
{
public:
    vector<int> keysRepartition;
    vector<map<string, int> > histograms;

    HashTable(string name = "", vector<int> keysRepartition = vector<int>());

    size_t getHash(string key);
    size_t getMultikeyHash(Couple &couple);

    vector<string> get(string key);
    virtual void put(Couple couple);

    void putMultikey(Couple couple);
    vector<Couple> fetchCouples(size_t keyHash, int keyHashSize, int position, size_t keyHash2 = 0, int keyHashSize2 = 0, int position2 = 0);

    virtual void printState();

    int getNumberBucketFetch();
    void setNumberBucketFetch(int bucketFetch);

    string getBucketPath();
    string getName();
    int getGlobalDepthLimit();

    void incNumberChain(int chainCount);

    virtual void addBHF();
    virtual bool canAddBHF();

protected:
    string bucketPath;
    string name;
    boost::hash<string> string_hasher;
    int globalDepthLimit;
    int numberBucketFetch;
    int numberItems;

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
        ar & globalDepthLimit;
    }
};

#endif // HASHTABLE_H
