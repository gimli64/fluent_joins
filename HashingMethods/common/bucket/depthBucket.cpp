#include "depthbucket.h"

DepthBucket::DepthBucket()
    :localDepth(0), Bucket()
{
}

DepthBucket::DepthBucket(HashingMethod *hasher)
    :localDepth(0), Bucket(hasher)
{
}

int DepthBucket::getLocalDepth() {
    return localDepth;
}

void DepthBucket::setLocalDepth(int depth) {
    localDepth = depth;
}

string DepthBucket::className() const
{
    return "DepthBucket ";
}

ostream& DepthBucket::dump(ostream &strm) const
{
    ostream& output = Bucket::dump(strm);
    return output << " , depth : " << localDepth;
}
