#include "hashingMethod.h"

HashingMethod::HashingMethod(string name, vector<int> keysRepartition, int relationSize)
    :numberItems(0), name(name), keysRepartition(keysRepartition), relationSize(relationSize)
{
    bucketPath = name + "/";
}

size_t HashingMethod::getHash(string key)
{
    return CityHash32(key.c_str(), key.size());
}

void HashingMethod::getHashes(size_t keyHash, int position, vector<size_t> &hashes)
{
    int numberBitsToSet = keysRepartition[position];

    int numberBitsUnset = 0;
    int numberBitsLeft = 0;
    for (int i = 0; i < position; i++) {
        numberBitsUnset += keysRepartition[i];
        numberBitsLeft += keysRepartition[i];
    }

    int numberBitsRight = 0;
    for (int i = position + 1; i < keysRepartition.size(); i++) {
        numberBitsUnset += keysRepartition[i];
        numberBitsRight += keysRepartition[i];
    }

    size_t hash = 0;
    size_t result = 0;
    size_t leftMask = ((1 << numberBitsLeft) - 1) << (numberBitsRight);
    hashes.reserve(numberBitsToSet + numberBitsUnset);
    for (int i = (int) pow(2.0, (double) numberBitsUnset) - 1; i >= 0; i--) {
        hash = 0;
        result = 0;
        hash += ((i & leftMask) >> numberBitsRight);
        hash <<= numberBitsToSet;
        hash += keyHash;
        hash <<= numberBitsRight;
        hash += i & ((1 << numberBitsRight) - 1);
        MurmurHash3_x86_32(&hash, sizeof(size_t), (uint32_t) 0, &result );
        hashes.push_back(result);
    }
}

vector<Bucket *> HashingMethod::fetchBuckets(size_t keyHash, int position)
{
    vector<Bucket *> buckets;
    vector<size_t> hashes;
    getHashes(keyHash, position, hashes);
    for (int i = 0; i < hashes.size(); i++) {
        buckets.push_back(fetchBucket(hashes[i]));
    }
    return buckets;
}

Bucket *HashingMethod::fetchBucket(size_t hash)
{
    return new Bucket();
}

void HashingMethod::insert(Couple couple)
{
    vector<size_t> hashes;
    for (int i = 0; i < couple.values.size(); i++)
        hashes.push_back(getHash(couple.values[i]));

    putCouple(interleaveHashes(hashes), couple);
}

size_t HashingMethod::interleaveHashes(vector<size_t> &hashes)
{
    size_t key = 0;

    for (int i = 0; i < hashes.size() - 1; i++) {
        key += hashes[i] & ((1 << keysRepartition[i]) - 1);
        key <<= keysRepartition[i + 1];
    }
    key += hashes[hashes.size() - 1] & ((1 << keysRepartition[hashes.size() - 1]) - 1);

    size_t result = 0;
    MurmurHash3_x86_32(&key, sizeof(size_t), (uint32_t) 0, &result );
    return result;
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

string HashingMethod::getName()
{
    return name;
}

int HashingMethod::getRelationSize()
{
    return relationSize;
}

int HashingMethod::getNumberDirEntries()
{
    return 0;
}

void HashingMethod::setNumberDirEntries(int number)
{
}

string HashingMethod::getBucketPath()
{
    return bucketPath;
}
