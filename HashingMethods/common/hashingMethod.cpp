#include "hashingMethod.h"

HashingMethod::HashingMethod()
    :numberBuckets(0), numberItems(0)
{
}

size_t HashingMethod::getHash(string value)
{
    return hasher(value);
}

int HashingMethod::getNumberBuckets()
{
    return numberBuckets;
}

void HashingMethod::setNumberBuckets(int number)
{
    numberBuckets = number;
}

string* HashingMethod::get(string value)
{
    return getValue(getHash(value), value);
}

void HashingMethod::put(string value)
{
    putValue(getHash(value), value);
}

string* HashingMethod::getValue(size_t key, string value)
{
    return (string*) NULL;
}

void HashingMethod::putValue(size_t key, string value)
{
}


