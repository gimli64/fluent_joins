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
    vector<int> BHFsRepartitions;
    vector<map<string, int> > histograms;
    string name;

    HashTable(string name, vector<int> BHFsRepartitions);

    vector<string> get(string key);
    void put(Couple couple);

    size_t getMultikeyHash(Couple &couple);

    int getNumberBHFs();
    void addBHF();

    virtual void printState() = 0;

protected:
    vector<int> interleaveOrder;
    int numberItems;

    void getHashes(size_t keyHash, int keyHashSize, int position, vector<size_t> &hashes);

private:
    boost::hash<string> string_hasher;
    boost::hash<int> int_hasher;

    virtual void putCouple(size_t hash, Couple couple) = 0;
    virtual vector<string> getValue(size_t hash) = 0;

    size_t getHash(string key);
    size_t interleaveHashes(vector<size_t> &hashes);
};

#endif // HASHTABLE_H
