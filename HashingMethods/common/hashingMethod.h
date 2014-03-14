#ifndef HASHINGMETHOD_H
#define HASHINGMETHOD_H
#include <boost/functional/hash.hpp>

using std::string;
using std::size_t;

class HashingMethod
{
public:
    HashingMethod();
    size_t getHash(string value);

private:
    boost::hash<string> hasher;
};

#endif // HASHINGMETHOD_H
