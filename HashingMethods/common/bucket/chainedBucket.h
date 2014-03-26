#ifndef CHAINEDBUCKET_H
#define CHAINEDBUCKET_H

#include "bucket.h"
#include "bucketFactory.h"

#include <boost/serialization/base_object.hpp>

class ChainedBucket : public Bucket
{
public:
    ChainedBucket();
    string getValue(string key);
    void putCouple(Couple couple);
    int getChainCount();
    vector<Couple> getAllValues();

    virtual string className() const;
    virtual ostream& dump(ostream &strm) const;

private:
    ChainedBucket* nextBucket;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Bucket>(*this);
        ar & nextBucket;
    }
};

#endif // CHAINEDBUCKET_H
