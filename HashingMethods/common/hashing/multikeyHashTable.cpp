#include "multikeyHashTable.h"

MultikeyHashTable::MultikeyHashTable(string name, vector<int> keysRepartition)
    :keysRepartition(keysRepartition), HashTable(name)
{
    leftMostBitIndex = -1;  // coherence with 0 indexation
    for (int i = 0; i < keysRepartition.size(); i++) {
        leftMostBitIndex += keysRepartition[i];
    }
}

vector<Bucket *> MultikeyHashTable::fetchBuckets(size_t keyHash, int keyHashSize, int position)
{
    vector<Bucket *> buckets;
    vector<size_t> hashes;
    getHashes(keyHash, keyHashSize, position, hashes);
    for (int i = 0; i < hashes.size(); i++) {
        buckets.push_back(fetchBucket(hashes[i]));
    }
    return buckets;
}

void MultikeyHashTable::getHashes(size_t keyHash, int keyHashSize, int position, vector<size_t> &hashes)
{
    int numberBitsToSet = keyHashSize;

    int numberBitsUnset = 0;
    int numberBitsLeft = 0;
    for (int i = 0; i < position; i++) {
        numberBitsUnset += keysRepartition[i];
        numberBitsLeft += keysRepartition[i];
    }
    numberBitsUnset += (keysRepartition[position] - keyHashSize);
    numberBitsLeft += (keysRepartition[position] - keyHashSize);

    int numberBitsRight = 0;
    for (int i = position + 1; i < keysRepartition.size(); i++) {
        numberBitsUnset += keysRepartition[i];
        numberBitsRight += keysRepartition[i];
    }

    size_t hash = 0;
    size_t result = 0;
    size_t leftMask = ((1 << numberBitsLeft) - 1) << (numberBitsRight);
    int numberHashes = (int) pow(2.0, (double) numberBitsUnset) - 1;
    hashes.reserve(numberHashes);
    for (int i = numberHashes; i >= 0; i--) {
        hash = 0;
        result = 0;
        hash += ((i & leftMask) >> numberBitsRight);
        hash <<= numberBitsToSet;
        hash += keyHash;
        hash <<= numberBitsRight;
        hash += i & ((1 << numberBitsRight) - 1);
        hashes.push_back(hash);
    }
}

Bucket *MultikeyHashTable::fetchBucket(size_t hash)
{
    return new Bucket();
}

void MultikeyHashTable::put(Couple couple)
{
//    cout << getHash(couple) << endl;
    putCouple(getHash(couple), couple);
}

size_t MultikeyHashTable::getHash(Couple couple)
{
    vector<size_t> hashes;
    for (int i = 0; i < couple.values.size(); i++)
        hashes.push_back(HashTable::getHash(couple.values[i]));

    return interleaveHashes(hashes);
}


size_t MultikeyHashTable::interleaveHashes(vector<size_t> &hashes)
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

    return key;
}
