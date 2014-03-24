#ifndef BUCKET_H
#define BUCKET_H

#include <vector>
#include <boost/serialization/vector.hpp>

using namespace std;

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
    int getChainCount();
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
