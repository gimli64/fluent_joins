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
        hashes.push_back(MurmurHash2(&hash, sizeof(hash), 0));
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
    return MurmurHash2(&key, sizeof(key), 0);
}

unsigned int MultikeyHashTable::MurmurHash2 ( const void * key, int len, unsigned int seed )
{
    // 'm' and 'r' are mixing constants generated offline.
    // They're not really 'magic', they just happen to work well.

    const unsigned int m = 0x5bd1e995;
    const int r = 24;

    // Initialize the hash to a 'random' value

    unsigned int h = seed ^ len;

    // Mix 4 bytes at a time into the hash

    const unsigned char * data = (const unsigned char *)key;

    while(len >= 4)
    {
        unsigned int k = *(unsigned int *)data;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    // Handle the last few bytes of the input array

    switch(len)
    {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0];
            h *= m;
    };

    // Do a few final mixes of the hash to ensure the last few
    // bytes are well-incorporated.

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}
