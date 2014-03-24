#include "extendibleHashing.h"

ExtendibleHashing::ExtendibleHashing()
    :directory(Directory(this)), HashingMethod()
{
}

string ExtendibleHashing::getValue(size_t key, string value)
{
    try {
        return directory.getValue(key, value);
    } catch (string &e) {
        throw e;
    }
}

void ExtendibleHashing::putValue(size_t key, string value)
{
    directory.putValue(key, value);
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
