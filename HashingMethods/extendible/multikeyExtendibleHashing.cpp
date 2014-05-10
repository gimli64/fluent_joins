#include "multikeyExtendibleHashing.h"

MultikeyExtendibleHashing::MultikeyExtendibleHashing(string name, vector<int> keysRepartition)
    :ExtendibleHashing(name, keysRepartition)
{
}

ostream& operator<<(ostream& strm, const MultikeyExtendibleHashing& hash)
{
    return hash.dump(strm);
}

