#ifndef BUCKET_H
#define BUCKET_H

#include "common/structure/couple.h"

#include <vector>
#include <boost/serialization/vector.hpp>

using namespace std;

class Bucket
{
public:
    static const int BUCKET_SIZE = 50;
    string name;

    Bucket();
    vector<string> getValue(string key);
    void putCouple(Couple couple);
    bool isFull();
    int size();
    int getChainCount();
    vector<Couple>& getAllValues();

    virtual string className() const;
    virtual ostream& dump(ostream &strm) const;

private:
    vector<Couple> elements;

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
