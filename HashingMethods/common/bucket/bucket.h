#ifndef BUCKET_H
#define BUCKET_H

#include "common/hashingMethod.h"

#include <sstream>
#include <fstream>
#include <vector>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/lexical_cast.hpp>

using std::string;
using std::vector;
using std::find;
using std::ostream;
using std::stringstream;

class Bucket
{
public:
    static const int BUCKET_SIZE = 10;
    string name;

    Bucket();
    string getValue(string value);
    void putValue(string value);
    bool isFull();
    int size();
    vector<string>& getAllValues();

    virtual string className() const;
    virtual ostream& dump(ostream &strm) const;

private:
    vector<string> elements;

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
