#ifndef COUPLE_H
#define COUPLE_H

#include <string>
#include <boost/serialization/vector.hpp>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

class Couple
{
public:
    Couple();
    Couple(string key, const tuple &tuple);
    Couple(string key, vector<string> &values);
    string key;
    vector<string> values;

    virtual string className() const;
    virtual ostream& dump(ostream &strm) const;

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & key;
        ar & values;
    }
};

bool operator==(const Couple &lhs, const string key);

ostream& operator<<(ostream&, const Couple&);

#endif // COUPLE_H
