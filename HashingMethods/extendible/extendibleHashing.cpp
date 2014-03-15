#include "extendibleHashing.h"

ExtendibleHashing::ExtendibleHashing()
    :directory(Directory(*this))
{
}

string* ExtendibleHashing::get(size_t key, string value)
{
    return directory.getValue(key, value);
}

void ExtendibleHashing::put(size_t key, string value)
{
    directory.putValue(key, value);
}
