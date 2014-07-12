#include "hashTable.h"

HashTable::HashTable(string name, vector<int> BHFsRepartitions)
    :name(name), BHFsRepartitions(BHFsRepartitions), numberItems(0)
{
    interleaveOrder = vector<int>();
    for (int i = 0; i < BHFsRepartitions.size(); i++) {
        histograms.push_back(map<string, int>());
        if (BHFsRepartitions[i] > 0)
            interleaveOrder.push_back(i);
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
    couple.key = hash_value;
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

vector<string> HashTable::get(string key)
{
    try {
        return getValue(getHash(key));
    } catch (string &e) {
        throw e;
    }
}

void HashTable::put(Couple couple)
{
    numberItems++;
    for (int i = 0; i < BHFsRepartitions.size(); i++) {
        if (!histograms[i][couple.values[i]]) {
            histograms[i][couple.values[i]] = 1;
        } else {
            histograms[i][couple.values[i]]++;
        }
    }
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

void HashTable::addBHF() {
    double maxNeededBHFRatio = 0.0;
    int maxNeededBHFRatioIndex = 0;
    int totalBHFsRepartitions = 0;

//    for (int i = 0; i < BHFsRepartitions.size(); i++) {
//        totalBHFsRepartitions += BHFsRepartitions[i];
//    }

//    for (int i = 0; i < BHFsRepartitions.size(); i++) {
//        if (BHFsRepartitions[i] > 0) {
//            int neededNumberBuckets = (double) histograms[i].size() / (Bucket::BUCKET_SIZE);
//            double neededBHFRatio = ((double) log(neededNumberBuckets) / log(2) - BHFsRepartitions[i]);
////            neededBHFRatio *= (totalBHFsRepartitions / BHFsRepartitions[i]);
//            cout << "column " << i << " neededBHFRatio : " << neededBHFRatio << endl;
//            if (neededBHFRatio > maxNeededBHFRatio) {
//                maxNeededBHFRatio = neededBHFRatio;
//                maxNeededBHFRatioIndex = i;
//            }
//        }
//    }

//    if (maxNeededBHFRatio <= 0.0 or totalBHFsRepartitions > directory.getDepth()) {
//        cout << "Not Adding BHF " << endl;
//    } else {
//        BHFsRepartitions[maxNeededBHFRatioIndex] += 1;
//        interleaveOrder.push_back(maxNeededBHFRatioIndex);
//        cout << "Adding BHF on column " << maxNeededBHFRatioIndex << endl;
//    }
    BHFsRepartitions[0] += 1;
    interleaveOrder.push_back(0);
}
