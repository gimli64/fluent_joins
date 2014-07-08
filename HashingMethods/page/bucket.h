#ifndef BUCKET_H
#define BUCKET_H

#include "page.h"
#include "common/couple.h"

#include <vector>
#include <boost/serialization/vector.hpp>

using namespace std;

class Bucket: public Page
{
public:
    static int BUCKET_SIZE;
    string name;

    Bucket(string name = "");
    virtual vector<string> getValue(string key);
    virtual void putCouple(Couple couple);
    bool isFull();
    void setBucketPath(string path);

    int size();
    vector<Couple> getAllValues();

private:
    string bucketPath;
    vector<Couple> elements;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & elements;
        ar & bucketPath;
    }
};

#endif // BUCKET_H
