#ifndef BUCKET_H
#define BUCKET_H

#include "couple.h"

#include <vector>
#include <boost/serialization/vector.hpp>

using namespace std;

class Bucket
{
public:
    static int BUCKET_SIZE;
    string name;
    vector<Couple> elements;

    Bucket(string name = "");
    virtual vector<string> getValue(string key);
    void putCouple(Couple couple);

    bool isFull();
    virtual int size();

    virtual int getChainCount();
    virtual vector<Couple> getAllValues();

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & elements;
    }
};

#endif // BUCKET_H
