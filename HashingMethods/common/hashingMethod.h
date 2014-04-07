#ifndef HASHINGMETHOD_H
#define HASHINGMETHOD_H

#include "common/structure/couple.h"
#include <boost/serialization/string.hpp>
#include <city.h>

using namespace std;

class HashingMethod
{
public:
    HashingMethod();
    HashingMethod(string name);
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

private:
    virtual vector<string> getValue(size_t hash, string key);
    virtual void putCouple(size_t hash, Couple couple);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & bucketPath;
    }
};

#endif // HASHINGMETHOD_H
