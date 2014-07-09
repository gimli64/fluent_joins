#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "common/couple.h"

#include <cstdlib>      // std::rand, std::srand
#include <vector>
#include <functional>
#include <boost/functional/hash.hpp>
#include <boost/lexical_cast.hpp>
#include <math.h>

using namespace std;
using namespace boost;

class HashTable
{
public:
    vector<int> keysRepartition;
    vector<int> interleaveOrder;
    vector<map<string, int> > histograms;
    string name;

    HashTable(string name = "", vector<int> keysRepartition = vector<int>(), vector<int> interleaveOrder = vector<int>());

    size_t getHash(string key);
    vector<string> get(string key);
    void put(Couple couple);

    size_t getMultikeyHash(Couple &couple);

    virtual void printState();

    int getNumberBucketFetch();
    void setNumberBucketFetch(int number);

    int getGlobalDepthLimit();
    string getName();

    virtual bool addBHF();

protected:
    boost::hash<string> string_hasher;
    boost::hash<int> int_hasher;
    int globalDepthLimit;
    int numberBucketFetch;
    int numberItems;

    void getHashes(size_t keyHash, int keyHashSize, int position, vector<size_t> &hashes);

private:
    size_t interleaveHashes(vector<size_t> &hashes);

    virtual void putCouple(size_t hash, Couple couple);
    virtual vector<string> getValue(size_t hash, string key);
};

#endif // HASHTABLE_H
