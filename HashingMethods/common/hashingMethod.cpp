#include "hashingMethod.h"

HashingMethod::HashingMethod()
{
}

size_t HashingMethod::getHash(string value)
{
    return hasher(value);
}
