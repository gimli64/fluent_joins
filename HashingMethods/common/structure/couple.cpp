#include "couple.h"

Couple::Couple()
{
}

Couple::Couple(string key, string value)
    :key(key), value(value)
{
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
