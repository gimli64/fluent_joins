#include "hashingMethod.h"

HashingMethod::HashingMethod()
    :numberItems(0), numberBuckets(0)
{
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

int HashingMethod::getNumberBuckets()
{
    return numberBuckets;
}

void HashingMethod::setNumberBuckets(int number)
{
    numberBuckets = number;
}


