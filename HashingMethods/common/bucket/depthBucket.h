#ifndef DEPTHBUCKET_H
#define DEPTHBUCKET_H
#include "bucket.h"

class DepthBucket : public Bucket
{
public:
    DepthBucket();
    DepthBucket(HashingMethod& hasher);

    int getLocalDepth();
    void setLocalDepth(int depth);

    virtual string className() const;
    virtual ostream& dump(ostream &strm) const;
private:
    int localDepth;
};

#endif // DEPTHBUCKET_H
