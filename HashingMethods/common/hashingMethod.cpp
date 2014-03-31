#include "hashingMethod.h"

HashingMethod* HashingMethod::instance = 0;

HashingMethod::HashingMethod()
    :numberItems(0)
{
}

HashingMethod* HashingMethod::getInstance()
{
    if (!instance)
        instance = new HashingMethod();
    return instance;
}

size_t HashingMethod::getHash(string key)
{
    return hasher(key);
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

int HashingMethod::getNumberDirEntries()
{
    return 0;
}

void HashingMethod::setNumberDirEntries(int number)
{
}
