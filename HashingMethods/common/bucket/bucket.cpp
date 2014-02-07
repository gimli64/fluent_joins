#include "bucket.h"

Bucket::Bucket()
    :elements(BUCKET_SIZE)
{
}

string Bucket::getValue(string value)
{
    vector<string>::iterator it = find(elements.begin(), elements.end(), value);
    if (it != elements.end()) {
        return *it;
    }
    return NULL;
}

void Bucket::putValue(string value)
{
    elements.push_back(value);
}

bool Bucket::isFull()
{
    return (this->size() >= BUCKET_SIZE);
}

int Bucket::size()
{
    return elements.size();
}
