#ifndef BUCKET_H
#define BUCKET_H

#include "page.h"
#include "common/couple.h"

#include <vector>

using namespace std;

class Bucket: public Page
{
public:
    static const int BUCKET_SIZE = 49;

    Bucket(string name = "");

    vector<string> getValue(string key);
    void putCouple(Couple couple);

    virtual bool isBucket();
    virtual Page* getBucket(size_t hash);

    bool isFull();
    int size();
    vector<Couple> getAllValues();

private:
    string name;
    vector<Couple> elements;
};

#endif // BUCKET_H
