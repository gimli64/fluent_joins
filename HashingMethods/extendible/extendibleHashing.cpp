#include "extendibleHashing.h"

ExtendibleHashing::ExtendibleHashing(string name, vector<int> keysRepartition)
    :directory(Directory(this)), HashTable(name, keysRepartition)
{
}

vector<string> ExtendibleHashing::getValue(size_t hash, string key)
{
    try {
        return directory.getValue(hash, key);
    } catch (string &e) {
        throw e;
    }
}

vector<DepthBucket *> ExtendibleHashing::getBuckets()
{
    return directory.getBuckets();
}

void ExtendibleHashing::clearBuckets()
{
    directory.clearBuckets();
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
