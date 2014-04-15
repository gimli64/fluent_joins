#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "common/structure/couple.h"
#include "common/bucket/bucket.h"
#include <vector>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/functional/hash.hpp>
#include <city.h>
#include <math.h>
#include "common/hashing/MurmurHash3.h"

using namespace std;

class HashTable
{
public:
    HashTable(string name = "");

    size_t getHash(string key);
    virtual size_t getHash(Couple couple);

    vector<string> get(string key);
    virtual void put(Couple couple);

    virtual int getNumberDirEntries();
    virtual void setNumberDirEntries(int number);
    int getNumberBucketFetch();

    string getBucketPath();
    string getName();

protected:
    string bucketPath;
    string name;
    int numberItems;
    int numberBucketFetch;

    virtual void putCouple(size_t hash, Couple couple);

private:
    boost::hash<size_t> simple_hasher;

    virtual vector<string> getValue(size_t hash, string key);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & bucketPath;
    }
};

#endif // HASHTABLE_H
