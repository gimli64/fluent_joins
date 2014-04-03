#ifndef HASHINGMETHOD_H
#define HASHINGMETHOD_H

#include "common/structure/couple.h"
#include <city.h>

using namespace std;

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
    virtual vector<string> getValue(size_t hash, string key);
    virtual void putCouple(size_t hash, Couple couple);

protected:
    int numberItems;
};

#endif // HASHINGMETHOD_H
