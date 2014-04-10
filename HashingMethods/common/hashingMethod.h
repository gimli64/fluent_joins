#ifndef HASHINGMETHOD_H
#define HASHINGMETHOD_H

#include "common/structure/couple.h"
#include <vector>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>
#include <city.h>

using namespace std;

class HashingMethod
{
public:
    HashingMethod(string name = "", vector<int> keysRepartition = vector<int>());
    size_t getHash(string key);
    vector<string> get(string key);
    void put(Couple couple);

    virtual int getNumberDirEntries();
    virtual void setNumberDirEntries(int number);

    string getBucketPath();
    string getName();

protected:
    string bucketPath;
    string name;
    int numberItems;
    vector<int> keysRepartition;

private:
    virtual vector<string> getValue(size_t hash, string key);
    virtual void putCouple(size_t hash, Couple couple);

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
