#include "hashTable.h"

HashTable::HashTable(string name, vector<int> BHFsRepartitions, vector<int> interleaveOrder)
    :name(name), BHFsRepartitions(BHFsRepartitions), numberItems(0), interleaveOrder(interleaveOrder)
{
    if (this->interleaveOrder.size() == 0) {
        for (int i = 0; i < BHFsRepartitions.size(); i++) {
            if (BHFsRepartitions[i] > 0) {
                this->interleaveOrder.push_back(i);
            }
        }
    }
}

size_t HashTable::getHash(string key)
{
    srand(int_hasher(lexical_cast<int>(key)));
    return rand();
}

size_t HashTable::getMultikeyHash(Couple& couple)
{
    vector<size_t> hashes;
    for (int i = 0; i < BHFsRepartitions.size(); i++) {
        if (BHFsRepartitions[i] > 0) {
            hashes.push_back(getHash(couple.values[i]));
        }
        else
            hashes.push_back(0);
    }

    size_t hash_value = interleaveHashes(hashes);
    return hash_value;
}

size_t HashTable::interleaveHashes(vector<size_t> &hashes)
{
    size_t key = 0;
    int hashIndex = 0;
    int bitIndex = 0;
    for (bitIndex = 0; bitIndex < interleaveOrder.size(); bitIndex++) {
        hashIndex = interleaveOrder[bitIndex];
        key |= ((hashes[hashIndex] & 1) << bitIndex);
        hashes[hashIndex] >>= 1;
    }

    return key;
}

vector<string> HashTable::get(Couple couple)
{
    try {
        return getValue(getMultikeyHash(couple), couple.key);
    } catch (string &e) {
        throw e;
    }
}

void HashTable::put(Couple couple)
{
    numberItems++;
//    for (int i = 0; i < BHFsRepartitions.size(); i++) {
//        if (!histograms[i][couple.values[i]]) {
//            histograms[i][couple.values[i]] = 1;
//        } else {
//            histograms[i][couple.values[i]]++;
//        }
//    }
    putCouple(getMultikeyHash(couple), couple);
}

void HashTable::getHashes(vector<size_t> &setHashes, vector<int> &sizes, vector<size_t> &hashes)
{
    int numberBitsUnset = 0;
    vector<int> bitsToSet;

    for (int i = 0; i < BHFsRepartitions.size(); i++) {
        numberBitsUnset += (BHFsRepartitions[i] - sizes[i]);
        bitsToSet.push_back(BHFsRepartitions[i] - sizes[i]);
    }

    int numberHashes = (int) pow(2.0, (double) numberBitsUnset);
    hashes.reserve(numberHashes);
    for (int i = 0; i < numberHashes; i++) {
        vector<size_t> hashValues;
        size_t rightOffset = 0;
        size_t mask = 0;
        for (int j = 0; j < BHFsRepartitions.size(); j++) {
            mask = ((1 << bitsToSet[j]) - 1) << rightOffset;
            hashValues.push_back((((i & mask) >> rightOffset) << sizes[j]) + setHashes[j]);
            rightOffset += bitsToSet[j];
        }

        size_t hash = interleaveHashes(hashValues);
        hashes.push_back(hash);
    }
}

int HashTable::getNumberBHFs()
{
    return interleaveOrder.size();
}
