#include "hybridDirectory.h"

HybridDirectory::HybridDirectory()
    :numberBuckets(1), Directory()
{
}

HybridDirectory::HybridDirectory(HashTable *hasher)
    :numberBuckets(1), Directory(hasher)
{
    hasher->setNumberDirEntries(hasher->getNumberDirEntries() + 1);
}

void HybridDirectory::putCouple(size_t hash, Couple couple)
{
    DepthBucket *bucket = getBucket(hash);
    if (bucket->isFull() /*and (hasher->getLevel() - bucket->getLocalDepth()) > 0*/) {
        if (bucket->getLocalDepth() == globalDepth) {
            doubleSize();
        }
        if (bucket->getLocalDepth() < globalDepth) {
            split(bucket);
            bucket = getBucket(hash);
        }
    }
    bucket->putCouple(couple);
}

vector<Couple> HybridDirectory::popAllValues()
{
    vector<Couple> elements;
    set<DepthBucket*> uniqueBuckets = set<DepthBucket*>(buckets.begin(), buckets.end());
    for(set<DepthBucket*>::iterator it = uniqueBuckets.begin(); it != uniqueBuckets.end(); ++it) {
        vector<Couple> values = (*it)->getAllValues();
        elements.insert(elements.end(), values.begin(), values.end());
        factory->deleteBucket(*it);
    }

    return elements;
}

void HybridDirectory::split(DepthBucket* bucket)
{
    Directory::split(bucket);
    numberBuckets++;
}

size_t HybridDirectory::getHash(Couple couple)
{
    return hasher->getMultikeyHash(couple);
}

void HybridDirectory::doubleSize()
{
    hasher->setNumberDirEntries(hasher->getNumberDirEntries() + buckets.size());
    Directory::doubleSize();
}

int HybridDirectory::getNumberBuckets()
{
    return numberBuckets;
}

string HybridDirectory::className() const
{
    return "HybridDirectory ";
}
