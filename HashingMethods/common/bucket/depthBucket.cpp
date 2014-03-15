#include "depthbucket.h"

DepthBucket::DepthBucket()
    :localDepth(0)
{
}

DepthBucket::DepthBucket(HashingMethod& hasher)
    :localDepth(0), Bucket(hasher)
{
}

int DepthBucket::getLocalDepth() {
    return localDepth;
}

void DepthBucket::setLocalDepth(int depth) {
    localDepth = depth;
}
