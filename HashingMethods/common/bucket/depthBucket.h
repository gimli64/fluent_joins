#ifndef DEPTHBUCKET_H
#define DEPTHBUCKET_H

#include "bucket.h"
#include "bucketFactory.h"

#include <boost/serialization/base_object.hpp>

class DepthBucket : public Bucket
{
public:
    DepthBucket(string name = "");
    ~DepthBucket();

    virtual vector<string> getValue(string key);

    virtual int getChainCount();
    virtual vector<Couple> getAllValues();
    virtual int getLocalDepth();

    void setLocalDepth(int depth);
    void setBucketPath(string path);

    bool hasNext() const;
    void setNextBucketName(string name);
    string getNextBucketName();
    void setNext(DepthBucket * next);
    DepthBucket *getNext() const;

    virtual string className() const;
    virtual ostream& dump(ostream &strm) const;

private:
    int localDepth;
    string bucketPath;
    string nextBucketName;
    DepthBucket * nextBucket;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<Bucket>(*this);
        ar & bucketPath;
        ar & localDepth;
        ar & nextBucketName;
    }
};

#endif // DEPTHBUCKET_H
