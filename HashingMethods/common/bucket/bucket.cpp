#include "bucket.h"

Bucket::Bucket()
    :elements()
{
    elements.reserve(BUCKET_SIZE);
}

string* Bucket::getValue(string value)
{
    vector<string>::iterator it = find(elements.begin(), elements.end(), value);
    if (it != elements.end()) {
        return &(*it);
    }
    return NULL;
}

void Bucket::putValue(string value)
{
    elements.push_back(value);
    // TODO : raise an error if the bucket is full !
}

bool Bucket::isFull()
{
    return (size() >= BUCKET_SIZE);
}

int Bucket::size()
{
    return elements.size();
}
