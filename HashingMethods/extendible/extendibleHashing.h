#ifndef EXTENDIBLEHASHING_H
#define EXTENDIBLEHASHING_H
#include "common/hashingMethod.h"
#include "common/directory/directory.h"
#include <sstream>

using std::ostream;

class Directory;

class ExtendibleHashing : public HashingMethod
{
public:
    ExtendibleHashing();
    ExtendibleHashing(string name);
    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

    vector<DepthBucket *> &getBuckets();
    void clearBuckets();

private:
    Directory directory;
    virtual vector<string> getValue(size_t hash, string key);
    virtual void putCouple(size_t hash, Couple couple);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<HashingMethod>(*this);
        ar & directory;
    }
};

ostream& operator<<(ostream&, const ExtendibleHashing&);

#endif // EXTENDIBLEHASHING_H
