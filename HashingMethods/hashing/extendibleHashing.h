#ifndef EXTENDIBLEHASHING_H
#define EXTENDIBLEHASHING_H

#include "hashTable.h"
#include "page/directory.h"
#include <ostream>
#include <sstream>

using std::ostream;

class ExtendibleHashing : public HashTable
{
public:
    ExtendibleHashing(string name = "", vector<int> keysRepartition = vector<int>(), vector<int> interleaveOrder = vector<int>());

    virtual void printState();
    virtual bool addBHF();

    vector<Couple> getCouples(size_t keyHash, int keyHashSize, int position);

    vector<Couple> fetchCouples(size_t keyHash, int keyHashSize, int position);
    vector<Couple> fetchAllCouples();

    void dimensionStats(int position);

private:
    Directory directory;

    virtual vector<string> getValue(size_t hash, string key);
    virtual void putCouple(size_t hash, Couple couple);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<HashTable>(*this);
        ar & directory;
    }
};

#endif // EXTENDIBLEHASHING_H
