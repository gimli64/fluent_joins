#ifndef EXTENDIBLEHASHING_H
#define EXTENDIBLEHASHING_H
#include "hashingMethod.h"
#include "directory.h"
#include <sstream>

using std::ostream;

class ExtendibleHashing : public HashingMethod
{
public:
    static ExtendibleHashing *getInstance();
    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

private:
    Directory directory;
    static ExtendibleHashing *instance;

    ExtendibleHashing();
    virtual string getValue(size_t key, string value);
    virtual void putValue(size_t key, string value);
};

ostream& operator<<(ostream&, const ExtendibleHashing&);

#endif // EXTENDIBLEHASHING_H
