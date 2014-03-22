#include "chainedDirectory.h"

ChainedDirectory::ChainedDirectory(HashingMethod* hasher)
    :numberBuckets(1), numberDoubling(0), Directory(hasher), nextDirectory(0)
{
}

string* ChainedDirectory::getValue(size_t key, string value)
{
    string* result = getBucket(key)->getValue(value);
    if (result == (string*) NULL && nextDirectory)
        result = nextDirectory->getValue(key, value);
    return result;
}

void ChainedDirectory::split(DepthBucket* bucket)
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
    if (nextDirectory)
        result += nextDirectory->getNumberBuckets();

    return result;
}

int ChainedDirectory::getChainCount()
{
    if (nextDirectory)
        return 1 + nextDirectory->getChainCount();
    else
        return 1;
}

vector<string> ChainedDirectory::getAllValues()
{
    vector<string> elements;
//    for(vector<DepthBucket*>::iterator it = buckets.begin(); it != buckets.end(); ++it) {
//        vector<string>& values = (*it)->getAllValues();
//        elements.insert(elements.end(), values.begin(), values.end());
//    }

//    if (nextDirectory) {
//        vector<string> values = nextDirectory->getAllValues();
//        elements.insert(elements.end(), values.begin(), values.end());
//    }

    return elements;
}

void ChainedDirectory::putValue(size_t key, string value)
{
    DepthBucket *bucket = getBucket(key);

    if (!bucket->isFull()) {
        bucket->putValue(value);
    } else {
        if (canBeDoubled() || bucket->getLocalDepth() < globalDepth) {

            if (bucket->getLocalDepth() == globalDepth && canBeDoubled()) {
                doubleSize();
                numberDoubling += 1;
                bucket = getBucket(key);  // Needed because of buckets reallocation in memory
            }

            if (bucket->getLocalDepth() < globalDepth) {
                split(bucket);
                bucket = getBucket(key);
                bucket->putValue(value);
            }

        } else {
            if (!nextDirectory)
                nextDirectory = new ChainedDirectory(hasher);
            nextDirectory->putValue(key, value);
        }
    }
}

string ChainedDirectory::className() const
{
    return "ChainedDirectory ";
}

std::ostream& ChainedDirectory::dump(std::ostream& strm) const
{
    ostream& output = Directory::dump(strm);
    if (nextDirectory) {
        output << "\n --> ";
        return nextDirectory->dump(output);
    }
    return output;
}
