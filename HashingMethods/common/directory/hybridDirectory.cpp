#include "hybridDirectory.h"

HybridDirectory::HybridDirectory()
    :numberBuckets(1), Directory()
{
}

HybridDirectory::HybridDirectory(HashingMethod *hasher)
    :numberBuckets(1), Directory(), hasher(hasher)
{
    hasher->setNumberDirEntries(hasher->getNumberDirEntries() + 1);
}

vector<Couple> HybridDirectory::popAllValues()
{
    vector<Couple> elements;
    set<string> uniqueBuckets = set<string>(buckets.begin(), buckets.end());
    for(set<string>::iterator it = uniqueBuckets.begin(); it != uniqueBuckets.end(); ++it) {
        DepthBucket* bucket = factory->readBucket(*it);
        vector<Couple>& values = bucket->getAllValues();
        elements.insert(elements.end(), values.begin(), values.end());
        factory->deleteBucket(bucket);
    }

    return elements;
}

void HybridDirectory::split(DepthBucket* bucket)
{
    Directory::split(bucket);
    numberBuckets++;
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
