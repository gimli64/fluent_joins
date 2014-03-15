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
    string* get(string value);
    void put(string value);
    string* get(size_t key, string value);
    void put(size_t key, string value);

private:
    boost::hash<string> hasher;

protected:
    int numberBuckets;
    int numberItems;
};

#endif // HASHINGMETHOD_H
