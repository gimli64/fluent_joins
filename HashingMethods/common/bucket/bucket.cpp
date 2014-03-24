#include "bucket.h"

Bucket::Bucket()
    :elements(), name("buckets/bucket")
{
    elements.reserve(BUCKET_SIZE);
}

string Bucket::getValue(string value)
{
    vector<string>::iterator it = find(elements.begin(), elements.end(), value);
    if (it != elements.end()) {
        return *it;
    } else {
        throw string("Value wasn't found");
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

int Bucket::getChainCount()
{
    return 1;
}

string Bucket::className() const
{
    return "Bucket ";
}

ostream& Bucket::dump(ostream &strm) const
{
    string output = className() + name + " : [";
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
