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
    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

private:
    Directory directory;

    ExtendibleHashing();
    virtual string getValue(size_t key, string value);
    virtual void putValue(size_t key, string value);
};

ostream& operator<<(ostream&, const ExtendibleHashing&);

#endif // EXTENDIBLEHASHING_H
