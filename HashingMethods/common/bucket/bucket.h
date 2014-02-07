#ifndef BUCKET_H
#define BUCKET_H

#include <iostream>
#include <vector>

using std::string;
using std::vector;
using std::find;

class Bucket
{
public:
    static const int BUCKET_SIZE = 10;

    Bucket();
    string getValue(string value);
    void putValue(string value);
    bool isFull();
    int size();

private:
    vector<string> elements;
};

#endif // BUCKET_H
