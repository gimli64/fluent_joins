#include "bucket.h"

Bucket::Bucket(string name)
    :elements(), name(name)
{
    elements.reserve(BUCKET_SIZE);
}

vector<string> Bucket::getValue(size_t hash, string key)
{
    vector<Couple>::iterator it = find(elements.begin(), elements.end(), key);
    if (it != elements.end()) {
        return (*it).values;
    } else {
        throw string("Value wasn't found");
    }
}

void Bucket::putCouple(size_t hash, Couple couple)
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

bool Bucket::isBucket()
{
    return true;
}

Bucket *Bucket::getBucket(size_t hash)
{
    return this;
}

string Bucket::className() const
{
    return "Bucket ";
}

ostream& Bucket::dump(ostream& strm) const
{
    ostream& output = strm << className() << name << " depth " << lexical_cast<string>(depth);
    output << " : " << lexical_cast<string>(elements.size()) << " tuples";
    output << " : [";
    for(int i = 0; i < elements.size(); i++) {
        output << bitset<3>(elements.at(i).key);
        if (i < elements.size() - 1)
            output << ", ";
    }
    output << "]";
    return output << endl;
}
