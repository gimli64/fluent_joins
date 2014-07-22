#include "couple.h"

Couple::Couple()
{
}

Couple::Couple(vector<string>& values)
    :values(values)
{
}


Couple::Couple(const tuple &tuple)
{
    for(int i = 0; i < tuple.size(); i++) {
        values.push_back(tuple[i].c_str());
    }
}

bool operator==(const Couple &lhs, const string key)
{
    return lhs.key == key;
}

string Couple::className() const
{
    return "Couple ";
}

ostream& Couple::dump(ostream &strm) const
{
    return strm;
}

ostream& operator<<(ostream& o, const Couple& couple)
{
    return couple.dump(o);
}
