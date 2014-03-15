#include "hashingMethod.h"

HashingMethod::HashingMethod()
    :numberBuckets(0), numberItems(0)
{
}

size_t HashingMethod::getHash(string value)
{
    return hasher(value);
}

string* HashingMethod::get(string value)
{
    size_t key = getHash(value);
    return get(key, value);
}

void HashingMethod::put(string value)
{
    size_t key = getHash(value);
    put(key, value);
}

string* HashingMethod::get(size_t key, string value)
{
    return (string*) NULL;
}

void HashingMethod::put(size_t key, string value)
{
}


