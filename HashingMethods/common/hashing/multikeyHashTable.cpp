#include "multikeyHashTable.h"

MultikeyHashTable::MultikeyHashTable(string name, vector<int> keysRepartition)
    :keysRepartition(keysRepartition), HashTable(name)
{
}

vector<Bucket *> MultikeyHashTable::fetchBuckets(size_t keyHash, int position)
{
    vector<Bucket *> buckets;
    vector<size_t> hashes;
    getHashes(keyHash, position, hashes);
    for (int i = 0; i < hashes.size(); i++) {
        buckets.push_back(fetchBucket(hashes[i]));
    }
    return buckets;
}

Bucket *MultikeyHashTable::fetchBucket(size_t hash)
{
    return new Bucket();
}

void MultikeyHashTable::put(Couple couple)
{
    putCouple(getHash(couple), couple);
}

void MultikeyHashTable::getHashes(size_t keyHash, int position, vector<size_t> &hashes)
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

    size_t result = 0;
    MurmurHash3_x86_32(&key, sizeof(size_t), (uint32_t) 0, &result );
//    return simple_hasher(key);
    return result;
}
