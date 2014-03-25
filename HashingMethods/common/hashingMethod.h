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
    int numberDirEntries;
    HashingMethod();
    static HashingMethod* getInstance();
    size_t getHash(string value);
    string get(string value);
    void put(string value);

private:
    boost::hash<string> hasher;
    static HashingMethod *instance;

    virtual string getValue(size_t key, string value);
    virtual void putValue(size_t key, string value);

protected:
    int numberItems;
};

#endif // HASHINGMETHOD_H
