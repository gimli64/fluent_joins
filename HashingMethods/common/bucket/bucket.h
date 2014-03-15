#ifndef BUCKET_H
#define BUCKET_H

#include <iostream>
#include <vector>

#include "hashingMethod.h"

using std::string;
using std::vector;
using std::find;

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

private:
    vector<string> elements;

protected:
    HashingMethod hasher;
};

#endif // BUCKET_H
