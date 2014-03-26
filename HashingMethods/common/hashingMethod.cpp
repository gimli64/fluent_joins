#include "hashingMethod.h"

HashingMethod* HashingMethod::instance = 0;

HashingMethod::HashingMethod()
    :numberItems(0), numberDirEntries(0)
{
}

HashingMethod* HashingMethod::getInstance()
{
    if (!instance)
        instance = new HashingMethod();
    return instance;
}

size_t HashingMethod::getHash(string value)
{
    return hasher(value);
}

string HashingMethod::get(string value)
{
    try {
        return getValue(getHash(value), value);
    } catch (string &e) {
        throw e;
    }
}

void HashingMethod::put(string value)
{
    putValue(getHash(value), value);
}

string HashingMethod::getValue(size_t key, string value)
{
    throw string("Value wasn't found");
}

void HashingMethod::putValue(size_t key, string value)
{
}
