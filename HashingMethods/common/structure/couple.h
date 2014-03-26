#ifndef COUPLE_H
#define COUPLE_H

#include <string>
#include <boost/serialization/access.hpp>

using namespace std;

class Couple
{
public:
    Couple();
    Couple(string key, string value);
    string key;
    string value;

    virtual string className() const;
    virtual ostream& dump(ostream &strm) const;

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & key;
        ar & value;
    }
};

bool operator==(const Couple &lhs, const string key);

ostream& operator<<(ostream&, const Couple&);

#endif // COUPLE_H
