#include "hashTable.h"

HashTable::HashTable(string name, vector<int> keysRepartition)
    :numberItems(0), name(name), keysRepartition(keysRepartition), numberBucketFetch(0)
{
    bucketPath = name + "/";

    if (keysRepartition.size() > 0) {
        leftMostBitIndex = -1;  // coherence with 0 indexation
        for (int i = 0; i < keysRepartition.size(); i++) {
            leftMostBitIndex += keysRepartition[i];
        }
    }
}

size_t HashTable::getHash(string key)
{
    return simple_hasher(key.c_str());
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

vector<Bucket *> HashTable::fetchBuckets(size_t keyHash, int keyHashSize, int position, size_t keyHash2, int keyHashSize2, int position2)
{
    vector<Bucket *> buckets;
    vector<size_t> hashes;
    getHashes(keyHash, keyHashSize, position, keyHash2, keyHashSize2, position2, hashes);
    for (int i = 0; i < hashes.size(); i++) {
        buckets.push_back(fetchBucket(hashes[i]));
    }
    return buckets;
}

vector<Couple> HashTable::fetchCouples(size_t keyHash, int keyHashSize, int position, size_t keyHash2, int keyHashSize2, int position2)
{
    Bucket *bucket;
    vector<Couple> couples;
    vector<size_t> hashes;
    getHashes(keyHash, keyHashSize, position, keyHash2, keyHashSize2, position2, hashes);
    for (int i = 0; i < hashes.size(); i++) {
        bucket = fetchBucket(hashes[i]);
        couples.insert(couples.end(), bucket->getAllValues().begin(), bucket->getAllValues().end());
    }
    delete bucket;
    return couples;
}

void HashTable::getHashes(size_t keyHash, int keyHashSize, int position, size_t keyHash2, int keyHashSize2, int position2, vector<size_t> &hashes)
{
    int numberBitsToSet = keyHashSize;
    int numberBitsToSet2 = keyHashSize2;

    int numberBitsUnset = 0;
    int numberBitsLeft = 0;
    for (int i = 0; i < position; i++) {
        numberBitsUnset += keysRepartition[i];
        numberBitsLeft += keysRepartition[i];
    }
    numberBitsUnset += (keysRepartition[position] - keyHashSize);
    numberBitsLeft += (keysRepartition[position] - keyHashSize);

    int numberBitsMiddle = 0;
    if (position2 > 0) {
        for (int i = position + 1; i < position2; i++) {
            numberBitsUnset += keysRepartition[i];
            numberBitsMiddle += keysRepartition[i];
        }
        numberBitsUnset += (keysRepartition[position2] - keyHashSize2);
        numberBitsMiddle += (keysRepartition[position2] - keyHashSize2);
    } else {
        position2 = position;
    }

    int numberBitsRight = 0;
    for (int i = position2 + 1; i < keysRepartition.size(); i++) {
        numberBitsUnset += keysRepartition[i];
        numberBitsRight += keysRepartition[i];
    }

    size_t hash = 0;
    size_t result = 0;
    size_t leftMask = ((1 << numberBitsLeft) - 1) << (numberBitsMiddle + numberBitsRight);
    size_t middleMask = ((1 << numberBitsMiddle) - 1) << (numberBitsRight);
    int numberHashes = (int) pow(2.0, (double) numberBitsUnset) - 1;
    hashes.reserve(numberHashes);
    for (int i = numberHashes; i >= 0; i--) {
        hash = 0;
        result = 0;
        hash += ((i & leftMask) >> (numberBitsMiddle + numberBitsRight));
        hash <<= numberBitsToSet;
        hash += keyHash;
        hash <<= numberBitsMiddle;
        hash += ((i & middleMask)) >> (numberBitsRight);
        hash <<= numberBitsToSet2;
        hash += keyHash2;
        hash <<= numberBitsRight;
        hash += i & ((1 << numberBitsRight) - 1);
        hashes.push_back(hash);
    }
}

Bucket *HashTable::fetchBucket(size_t hash)
{
    return new Bucket();
}

void HashTable::putMultikey(Couple couple)
{
    putCouple(getMultikeyHash(couple), couple);
}

size_t HashTable::getMultikeyHash(Couple couple)
{
    vector<size_t> hashes;
    for (int i = 0; i < couple.values.size(); i++)
        hashes.push_back(getHash(couple.values[i]));

    return interleaveHashes(hashes);
}

size_t HashTable::interleaveHashes(vector<size_t> &hashes)
{
    size_t key = 0;

    for (int i = 0; i < hashes.size() - 1; i++) {
        key += hashes[i] & ((1 << keysRepartition[i]) - 1);
        key <<= keysRepartition[i + 1];
    }
    key += hashes[hashes.size() - 1] & ((1 << keysRepartition[hashes.size() - 1]) - 1);
    return key;

    //    vector<int> repartition = keysRepartition;
    //    int hashIndex = 0;
    //    int bitIndex = 0;
    //    while (bitIndex <= leftMostBitIndex) {
    //        if (repartition[hashIndex] > 0) {
    //            key |= ((hashes[hashIndex] & 1) << bitIndex);
    //            bitIndex += 1;
    //            hashes[hashIndex] >>= 1;
    //            repartition[hashIndex] -= 1;
    //        }
    //        hashIndex = (hashIndex + 1) % hashes.size();
    //    }

//    return key;
}

int HashTable::getNumberBucketFetch()
{
    return numberBucketFetch;
}

void HashTable::setNumberBucketFetch(int bucketFetch)
{
    numberBucketFetch = bucketFetch;
}

string HashTable::getBucketPath()
{
    return bucketPath;
}
