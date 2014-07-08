#ifndef CHAINEDBUCKET_H
#define CHAINEDBUCKET_H

#include "bucket.h"
#include "bucketFactory.h"

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/string.hpp>

class ChainedBucket : public Bucket
{
public:
    ChainedBucket(string name = "");
    ~ChainedBucket();

    vector<string> getValue(string key);
    void putCouple(Couple couple);

    int getChainCount();
    void setBucketPath(string path);

    virtual vector<Couple> getAllValues();

    virtual string className() const;
    virtual ostream& dump(ostream &strm) const;

    bool hasNext();
    ChainedBucket *next();
    string nextName();

private:
    string bucketPath;
    string nextBucketName;
    ChainedBucket* nextBucket;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Bucket>(*this);
        ar & bucketPath;
        ar & nextBucketName;
    }
};

#endif // CHAINEDBUCKET_H
