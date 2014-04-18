#include "hashTable.h"

HashTable::HashTable(string name)
    :numberItems(0), name(name), numberBucketFetch(0)
{
    bucketPath = name + "/";
}

size_t HashTable::getHash(string key)
{
    return simple_hasher(key.c_str());
}

size_t HashTable::getHash(Couple couple)
{
    return 0;
}

int HashTable::getNumberBucketFetch()
{
    return numberBucketFetch;
}

void HashTable::setNumberBucketFetch(int bucketFetch)
{
    numberBucketFetch = bucketFetch;
}

vector<string> HashTable::get(string key)
{
    try {
        return getValue(getHash(key), key);
    } catch (string &e) {
        throw e;
    }
}

void HashTable::put(Couple couple)
{
    putCouple(getHash(couple.key), couple);
}

vector<string> HashTable::getValue(size_t hash, string key)
{
    throw string("Value wasn't found");
}

void HashTable::putCouple(size_t hash, Couple couple)
{
}

string HashTable::getName()
{
    return name;
}

int HashTable::getNumberDirEntries()
{
    return 0;
}

void HashTable::setNumberDirEntries(int number)
{
}

string HashTable::getBucketPath()
{
    return bucketPath;
}
