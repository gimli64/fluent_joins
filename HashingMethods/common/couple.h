#ifndef COUPLE_H
#define COUPLE_H

#include <iostream>
#include <string>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;

class Couple
{
public:
    Couple();
    Couple(const tuple &tuple);
    Couple(vector<string> &values);

    string key;
    vector<string> values;

    virtual string className() const;
    virtual ostream& dump(ostream &strm) const;

private:
};

bool operator==(const Couple &lhs, const string key);

ostream& operator<<(ostream&, const Couple&);


class Comparator
{
public:
    Comparator(int position) :position(position) {}

    inline bool operator() (const Couple& couple1, const Couple& couple2) { return (couple1.values[position] < couple2.values[position]); }

private:
    int position;
};

#endif // COUPLE_H
