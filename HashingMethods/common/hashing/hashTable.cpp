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

void HashTable::addBHF() {}

void HashTable::printState() {}

vector<Couple> HashTable::fetchCouples(size_t keyHash, int keyHashSize, int position, size_t keyHash2, int keyHashSize2, int position2)
{
    Bucket *bucket;
    vector<Couple> couples;
    vector<size_t> hashes;
    getHashes(keyHash, keyHashSize, position, keyHash2, keyHashSize2, position2, hashes);
    for (int i = 0; i < hashes.size(); i++) {
        bucket = fetchBucket(hashes[i]);
        vector<Couple> values = bucket->getAllValues();
        couples.insert(couples.end(), values.begin(), values.end());
    }
    delete bucket;
    return couples;
}

size_t HashTable::interleaveHashes(vector<size_t> &hashes, vector<int> keysRepartition)
{
    size_t key = 0;

    int hashIndex = 0;
    int bitIndex = 0;
    while (bitIndex <= globalDepthLimit) {
//        if (keysRepartition[hashIndex] > 0) {
//            key |= ((hashes[hashIndex] & 1) << bitIndex);
//            bitIndex += 1;
//            hashes[hashIndex] >>= 1;
//            keysRepartition[hashIndex] -= 1;
//        }
//        hashIndex = (hashIndex + 1) % hashes.size();
        hashIndex = interleaveOrder[bitIndex];
        key |= ((hashes[hashIndex] & 1) << bitIndex);
        bitIndex += 1;
        hashes[hashIndex] >>= 1;
        keysRepartition[hashIndex] -= 1;
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

        hashes.push_back(interleaveHashes(hash_values, keysRepartition));
    }
}

Bucket *HashTable::fetchBucket(size_t hash)
{
    return new Bucket();
}

void HashTable::putMultikey(Couple couple)
{
    numberItems++;
    for (int i = 0; i < couple.values.size(); i++) {
        if (keysRepartition[i] > 0) {
            if (!histograms[i][couple.values[i]]) {
                histograms[i][couple.values[i]] = 1;
            } else {
                histograms[i][couple.values[i]]++;
            }
        }
    }
    putCouple(getMultikeyHash(couple), couple);
}

size_t HashTable::getMultikeyHash(Couple& couple)
{
    vector<size_t> hashes;
    for (int i = 0; i < couple.values.size(); i++) {
        hashes.push_back(getHash(couple.values[i]));
    }

    size_t hash_value = interleaveHashes(hashes, keysRepartition);
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
