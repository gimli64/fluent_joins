#include "bucket.h"

Bucket::Bucket()
    :elements()
{
    elements.reserve(BUCKET_SIZE);
}

Bucket::Bucket(HashingMethod* hasher)
    :elements(), hasher(hasher)
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

vector<string> &Bucket::getAllValues()
{
    return elements;
}

string Bucket::className() const
{
    return "Bucket ";
}

ostream& Bucket::dump(ostream &strm) const
{
    const void * address = static_cast<const void*>(this);
    stringstream ss;
    ss << address;
    string output = className() + ss.str() + " : [";
    for(int i = 0; i < elements.size(); i++) {
        output += elements.at(i);
        if (i < elements.size() - 1)
            output += ", ";
    }
    return strm << output << "]";
}

std::ostream& operator<<(std::ostream& o, const Bucket& bucket)
{
    return bucket.dump(o);
}
