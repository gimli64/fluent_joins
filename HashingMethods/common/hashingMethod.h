#ifndef HASHINGMETHOD_H
#define HASHINGMETHOD_H
#include <boost/functional/hash.hpp>

using std::cout;
using std::endl;
using std::string;
using std::size_t;

class HashingMethod
{
public:
    HashingMethod();
    size_t getHash(string value);
    string get(string value);
    void put(string value);
    int getNumberBuckets();
    void setNumberBuckets(int number);

private:
    boost::hash<string> hasher;
    virtual string getValue(size_t key, string value);
    virtual void putValue(size_t key, string value);

protected:
    int numberBuckets;
    int numberItems;
};

#endif // HASHINGMETHOD_H
