#ifndef DEPTHBUCKET_H
#define DEPTHBUCKET_H

#include "bucket.h"

#include <boost/serialization/base_object.hpp>

class DepthBucket : public Bucket
{
public:
    DepthBucket(string name = "");

    int getLocalDepth();
    void setLocalDepth(int depth);

    virtual string className() const;
    virtual ostream& dump(ostream &strm) const;

private:
    int localDepth;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Bucket>(*this);
        ar & localDepth;
    }
};

#endif // DEPTHBUCKET_H
