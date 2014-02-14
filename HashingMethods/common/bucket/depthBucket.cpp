#include "depthbucket.h"

DepthBucket::DepthBucket()
    :localDepth(0)
{
}

int DepthBucket::getLocalDepth() {
    return localDepth;
}

void DepthBucket::setLocalDepth(int depth) {
    localDepth = depth;
}
