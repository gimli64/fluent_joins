#ifndef BUCKET_H
#define BUCKET_H

#include "page.h"
#include "common/couple.h"

#include <vector>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;

class Bucket: public Page
{
public:
    static const int BUCKET_SIZE = 49;

    Bucket(string name = "");

    vector<string> getValue(size_t hash);
    void putCouple(size_t hash, Couple couple);

    Bucket *getBucket(size_t hash);
    bool isBucket();
    bool isFull();

    int size();
    vector<Couple> getAllValues();

    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

private:
    string name;
    vector<Couple> elements;
};

#endif // BUCKET_H
