#include "chainedDirectory.h"

ChainedDirectory::ChainedDirectory(HashingMethod &hasher)
    :numberBuckets(1), numberDoubling(1), Directory(hasher), nextDirectory(0)
{
}

string* ChainedDirectory::getValue(size_t key, string value)
{
    string* result = getBucket(key).getValue(value);
    if (result == (string*) NULL && nextDirectory != 0)
        result = nextDirectory->getValue(key, value);
    return result;
}

void ChainedDirectory::split(DepthBucket &bucket)
{
    Directory::split(bucket);
    numberBuckets++;
}

bool ChainedDirectory::canBeDoubled()
{
    return numberDoubling < MAX_DOUBLING;
}

int ChainedDirectory::getNumberBuckets()
{
    int result = numberBuckets;
    if (nextDirectory != 0)
        result += nextDirectory->getNumberBuckets();

    return result;
}

int ChainedDirectory::getChainCount()
{
    if (nextDirectory != 0)
        return 1 + nextDirectory->getChainCount();
    else
        return 1;
}

vector<string> ChainedDirectory::getAllValues()
{
    vector<string> elements;
    for(vector<DepthBucket>::iterator it = buckets.begin(); it != buckets.end(); ++it) {
        vector<string>& values = it->getAllValues();
        elements.insert(elements.end(), values.begin(), values.end());
    }

    if (nextDirectory != 0) {
        vector<string> values = nextDirectory->getAllValues();
        elements.insert(elements.end(), values.begin(), values.end());
    }

    return elements;
}

void ChainedDirectory::putValue(size_t key, string value)
{
    DepthBucket &bucket = getBucket(key);

    if (!bucket.isFull()) {
        bucket.putValue(value);
    } else {
        if (canBeDoubled() || bucket.getLocalDepth() < globalDepth) {

            if (bucket.getLocalDepth() == globalDepth && canBeDoubled()) {
                doubleSize();
                numberDoubling += 1;
            }

            if (bucket.getLocalDepth() < globalDepth) {
                split(bucket);
                bucket = getBucket(key);
                bucket.putValue(value);
            }

        } else {
            if (nextDirectory == 0)
                nextDirectory = new ChainedDirectory(hasher);
            nextDirectory->putValue(key, value);
        }
    }
}
