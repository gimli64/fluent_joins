#include "hashingMethod.h"

HashingMethod::HashingMethod()
    :numberItems(0)
{
}

HashingMethod::HashingMethod(string name)
    :numberItems(0), name(name)
{
    bucketPath = name + "/";
}

size_t HashingMethod::getHash(string key)
{
    return CityHash32(key.c_str(), key.size());
}

vector<string> HashingMethod::get(string key)
{
    try {
        return getValue(getHash(key), key);
    } catch (string &e) {
        throw e;
    }
}

void HashingMethod::put(Couple couple)
{
    putCouple(getHash(couple.key), couple);
}

vector<string> HashingMethod::getValue(size_t hash, string key)
{
    throw string("Value wasn't found");
}

void HashingMethod::putCouple(size_t hash, Couple couple)
{
}

string HashingMethod::getName()
{
    return name;
}

int HashingMethod::getNumberDirEntries()
{
    return 0;
}

void HashingMethod::setNumberDirEntries(int number)
{
}

string HashingMethod::getBucketPath()
{
    return bucketPath;
}
