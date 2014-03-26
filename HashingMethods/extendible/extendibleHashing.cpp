#include "extendibleHashing.h"

ExtendibleHashing::ExtendibleHashing()
    :directory(Directory()), HashingMethod()
{
}

string ExtendibleHashing::getValue(size_t hash, string key)
{
    try {
        return directory.getValue(hash, key);
    } catch (string &e) {
        throw e;
    }
}

void ExtendibleHashing::putCouple(size_t hash, Couple couple)
{
    directory.putCouple(hash, couple);
}

string ExtendibleHashing::className() const
{
    return "ExtendibleHashing ";
}

ostream& ExtendibleHashing::dump(ostream& strm) const
{
    const void * address = static_cast<const void*>(this);
    stringstream ss;
    ss << address;
    ostream& output = strm;
    output << className() + ss.str() + " : \n";
    output << "## " << directory;
    return output;
}

ostream& operator<<(ostream& strm, const ExtendibleHashing& hash)
{
    return hash.dump(strm);
}
