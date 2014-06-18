#include "hashTable.h"

HashTable::HashTable(string name, vector<int> keysRepartition, vector<int> order)
    :name(name), keysRepartition(keysRepartition), numberBucketFetch(0), numberItems(0), interleaveOrder(order)
{
    bucketPath = name + "/";

    if (keysRepartition.size() > 0) {
        globalDepthLimit = -1;  // coherence with 0 indexation
        for (int i = 0; i < keysRepartition.size(); i++) {
            histograms.push_back(map<string, int>());
            globalDepthLimit += keysRepartition[i];
            if (keysRepartition[i] > 0)
                interleaveOrder.push_back(i);
        }
    }
    if (interleaveOrder.size() == 0) {
        for (int i = 0; i < keysRepartition.size(); i++) {
            if (keysRepartition[i] > 0)
                interleaveOrder.push_back(i);
        }
    }
}

size_t HashTable::getHash(string key)
{
    return string_hasher(key);
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

void HashTable::putCouple(size_t hash, Couple couple) {}

bool HashTable::addBHF()
{
    return false;
}

void HashTable::printState() {}

size_t HashTable::interleaveHashes(vector<size_t> &hashes)
{
    size_t key = 0;

    int hashIndex = 0;
    int bitIndex = 0;
    for (bitIndex = 0; bitIndex <= globalDepthLimit; bitIndex++) {
        hashIndex = interleaveOrder[bitIndex];
        key |= ((hashes[hashIndex] & 1) << bitIndex);
        hashes[hashIndex] >>= 1;
    }

    return key;
}

void HashTable::getHashes(size_t keyHash, int keyHashSize, int position, size_t keyHash2, int keyHashSize2, int position2, vector<size_t> &hashes)
{
    int numberBitsUnset = 0;
    vector<int> bitsToSet;

    for (int i = 0; i < keysRepartition.size(); i++) {
        if (i == position) {
            numberBitsUnset += (keysRepartition[i] - keyHashSize);
            bitsToSet.push_back(keysRepartition[i] - keyHashSize);
        } else if (i == position2) {
            numberBitsUnset += (keysRepartition[i] - keyHashSize2);
            bitsToSet.push_back(keysRepartition[i] - keyHashSize2);
        } else {
            numberBitsUnset += keysRepartition[i];
            bitsToSet.push_back(keysRepartition[i]);
        }
    }

    int numberHashes = (int) pow(2.0, (double) numberBitsUnset) - 1;
    hashes.reserve(numberHashes);

    vector<size_t> hash_values;
    size_t rightOffset;
    size_t mask;
    for (int i = numberHashes; i >= 0; i--) {
        hash_values.clear();
        rightOffset = 0;
        mask = 0;

        for (int j = 0; j < keysRepartition.size(); j++) {
            mask = ((1 << bitsToSet[j]) - 1) << rightOffset;

            if (j == position) {
                hash_values.push_back((((i & mask) >> rightOffset) << keyHashSize) + keyHash);
            } else if (j == position2) {
                hash_values.push_back(((((i & mask) >> rightOffset) << keyHashSize2) + keyHash2));
            } else {
                hash_values.push_back((i & mask) >> rightOffset);
            }
            rightOffset += bitsToSet[j];
        }

        size_t hash = interleaveHashes(hash_values);
        hashes.push_back(hash);
    }
}

Bucket *HashTable::fetchBucket(size_t hash)
{
    return new Bucket();
}

void HashTable::putMultikey(Couple couple)
{
    numberItems++;
    for (int i = 0; i < keysRepartition.size(); i++) {
        if (!histograms[i][couple.values[i]]) {
            histograms[i][couple.values[i]] = 1;
        } else {
            histograms[i][couple.values[i]]++;
        }
    }
    putCouple(getMultikeyHash(couple), couple);
}

size_t HashTable::getMultikeyHash(Couple& couple)
{
    vector<size_t> hashes;
    for (int i = 0; i < keysRepartition.size(); i++) {
        hashes.push_back(getHash(couple.values[i]));
    }

    size_t hash_value = interleaveHashes(hashes);
    couple.key = lexical_cast<string>(hash_value);
    return hash_value;
}

int HashTable::getNumberBucketFetch()
{
    return numberBucketFetch;
}

void HashTable::setNumberBucketFetch(int number)
{
    numberBucketFetch = number;
}

string HashTable::getBucketPath()
{
    return bucketPath;
}

int HashTable::getGlobalDepthLimit()
{
    return globalDepthLimit;
}

string HashTable::getName()
{
    return name;
}
