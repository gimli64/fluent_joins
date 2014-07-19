#ifndef EXTENDIBLEHASHING_H
#define EXTENDIBLEHASHING_H

#include "experiments/manager.h"
#include "hashTable.h"
#include "page/directory.h"
#include <ostream>
#include <sstream>

using std::ostream;

class ExtendibleHashing : public HashTable
{
public:
    ExtendibleHashing(string name, vector<int> BHFsRepartitions, vector<int>interleaveOrder = vector<int>());

    virtual void printState();

    virtual void addBHF();
    virtual set<Bucket *> getBuckets(size_t keyHash, int keyHashSize, int position);

private:
    Directory directory;

    virtual vector<string> getValue(size_t hash);
    virtual void putCouple(size_t hash, Couple couple);
};

#endif // EXTENDIBLEHASHING_H
