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

    virtual ostream& dump(ostream &strm) const {
        const void * address = static_cast<const void*>(this);
        std::stringstream ss;
        ss << address;
        string output = "Bucket " + ss.str() + " : [";
        for(int i = 0; i < elements.size(); i++) {
            output += elements.at(i);
            if (i < elements.size() - 1)
                output = output += ", ";
        }
        return strm << output << "]";
    }

private:
    vector<string> elements;
    friend std::ostream& operator<<(std::ostream&, const Bucket&);

protected:
    HashingMethod hasher;
};

#endif // BUCKET_H
