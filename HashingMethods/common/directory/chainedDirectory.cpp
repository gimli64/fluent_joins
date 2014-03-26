#include "chainedDirectory.h"

ChainedDirectory::ChainedDirectory()
    :numberBuckets(1), Directory()
{
}

ChainedDirectory::ChainedDirectory(HashingMethod *hasher)
    :numberBuckets(1), Directory(), hasher(hasher)
{
    hasher->setNumberDirEntries(hasher->getNumberDirEntries() + 1);
}

vector<Couple> ChainedDirectory::popAllValues()
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

void ChainedDirectory::split(DepthBucket* bucket)
{
    Directory::split(bucket);
    numberBuckets++;
}

void ChainedDirectory::doubleSize()
{
    hasher->setNumberDirEntries(hasher->getNumberDirEntries() + buckets.size());
    Directory::doubleSize();
}

int ChainedDirectory::getNumberBuckets()
{
    return numberBuckets;
}

string ChainedDirectory::className() const
{
    return "ChainedDirectory ";
}
