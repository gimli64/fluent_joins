#ifndef BUCKET_H
#define BUCKET_H

#include <iostream>
#include <sstream>
#include <vector>

#include "hashingMethod.h"

using std::string;
using std::vector;
using std::find;
using std::ostream;
using std::stringstream;

class Bucket
{
public:
    static const int BUCKET_SIZE = 10;

    Bucket();
    Bucket(HashingMethod& hasher);
    ~Bucket();
    string* getValue(string value);
    void putValue(string value);
    bool isFull();
    int size();
    vector<string>& getAllValues();

    virtual string className() const;
    virtual ostream& dump(ostream &strm) const;

private:
    vector<string> elements;

protected:
    HashingMethod hasher;
};

std::ostream& operator<<(std::ostream&, const Bucket&);

#endif // BUCKET_H
