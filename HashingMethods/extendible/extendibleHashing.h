#ifndef EXTENDIBLEHASHING_H
#define EXTENDIBLEHASHING_H
#include "hashingMethod.h"
#include "directory.h"

class ExtendibleHashing : public HashingMethod
{
public:
    ExtendibleHashing();
    string* get(size_t key, string value);
    void put(size_t key, string value);

private:
    Directory directory;
};

#endif // EXTENDIBLEHASHING_H
