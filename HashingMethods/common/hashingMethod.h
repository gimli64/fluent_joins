#ifndef HASHINGMETHOD_H
#define HASHINGMETHOD_H

#include "common/structure/couple.h"
#include <boost/functional/hash.hpp>

using std::cout;
using std::endl;
using std::string;
using std::size_t;

class HashingMethod
{
public:
    HashingMethod();
    size_t getHash(string key);
    vector<string> get(string key);
    void put(Couple couple);

    virtual int getNumberDirEntries();
    virtual void setNumberDirEntries(int number);

    string getBucketPath();
    void setBucketPath(string path);

protected:
    string bucketPath;

private:
    boost::hash<string> hasher;

    virtual vector<string> getValue(size_t hash, string key);
    virtual void putCouple(size_t hash, Couple couple);

protected:
    int numberItems;
};

#endif // HASHINGMETHOD_H
