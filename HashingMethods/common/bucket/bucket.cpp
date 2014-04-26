#include "bucket.h"

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

string Bucket::className() const
{
    return "Bucket ";
}

ostream& Bucket::dump(ostream &strm) const
{
    string output = className() + name + " : [";
    for(int i = 0; i < elements.size(); i++) {
        output += elements.at(i).key;
        if (i < elements.size() - 1)
            output += ", ";
    }
    return strm << output << "]";
}

ostream& operator<<(ostream& o, const Bucket& bucket)
{
    return bucket.dump(o);
}
