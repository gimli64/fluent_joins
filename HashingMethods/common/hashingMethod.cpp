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

vector<size_t> HashingMethod::getHashes(string key, int position)
{
    vector<size_t> hashes;
    return hashes;
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
    bitset<32> bits;
    int bitIndex = 0;
    for (int i = 0; i < hashes.size(); i++) {
        for (int j = 1; j <= keysRepartition[i]; j++) {
            bits[bitIndex] = (hashes[i] & ((1 << j) - 1));
            bitIndex++;
        }
    }
    return bits.to_ulong();
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
