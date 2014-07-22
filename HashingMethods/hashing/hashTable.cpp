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
//    srand(std::hash<string>(key));
    return rand();
//    return int_hasher(lexical_cast<int>(key));
//    return string_hasher(key);
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

void HashTable::getHashes(size_t keyHash, int keyHashSize, int position, vector<size_t> &hashes)
{
    int numberBitsUnset = 0;
    vector<int> bitsToSet;

    for (int i = 0; i < BHFsRepartitions.size(); i++) {
        if (i == position) {
            numberBitsUnset += (BHFsRepartitions[i] - keyHashSize);
            bitsToSet.push_back(BHFsRepartitions[i] - keyHashSize);
        } else {
            numberBitsUnset += BHFsRepartitions[i];
            bitsToSet.push_back(BHFsRepartitions[i]);
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
        for (int j = 0; j < BHFsRepartitions.size(); j++) {
            mask = ((1 << bitsToSet[j]) - 1) << rightOffset;
            if (j == position) {
                hash_values.push_back((((i & mask) >> rightOffset) << keyHashSize) + keyHash);
            } else {
                hash_values.push_back((i & mask) >> rightOffset);
            }
            rightOffset += bitsToSet[j];
        }

        size_t hash = interleaveHashes(hash_values);
        hashes.push_back(hash);
    }
}

int HashTable::getNumberBHFs()
{
    return interleaveOrder.size();
}
