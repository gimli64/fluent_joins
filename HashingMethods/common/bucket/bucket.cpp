#include "bucket.h"

int Bucket::BUCKET_SIZE = 60;

Bucket::Bucket(string name)
    :elements(), name(name)
{
    elements.reserve(BUCKET_SIZE);
}

vector<string> Bucket::getValue(string key)
{
    vector<Couple>::iterator it = find(elements.begin(), elements.end(), key);
    if (it != elements.end()) {
        return (*it).values;
    } else {
        throw string("Value wasn't found");
    }
}

void Bucket::putCouple(Couple couple)
{
    elements.push_back(couple);
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

vector<Couple> Bucket::getAllValues()
{
    return elements;
}

int Bucket::getChainCount()
{
    return 1;
}
