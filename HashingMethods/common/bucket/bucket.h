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

    Bucket(string name = "");
    vector<string> getValue(string key);
    virtual void putCouple(Couple couple);

    bool isFull();
    int size();

    virtual int getChainCount();
    virtual vector<Couple> getAllValues();

    virtual string className() const;
    virtual ostream& dump(ostream &strm) const;

    vector<Couple> elements;

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & elements;
    }
};

ostream& operator<<(ostream&, const Bucket&);

#endif // BUCKET_H
