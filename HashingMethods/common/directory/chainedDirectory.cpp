#include "chainedDirectory.h"

ChainedDirectory::ChainedDirectory()
    :numberBuckets(1), numberDoubling(0), Directory(), nextDirectory(0)
{
}

ChainedDirectory::ChainedDirectory(HashingMethod *hasher)
    :numberBuckets(1), numberDoubling(0), Directory(hasher), nextDirectory(0)
{
    notifyNumberBuckets(1);
}


string ChainedDirectory::getValue(size_t key, string value)
{
    string result;
    DepthBucket *bucket = getBucket(key);
    try {
        result = bucket->getValue(value);
    } catch (string &e) {
        if (!nextDirectory) {
            throw e;
        } else {
            try {
                result = nextDirectory->getValue(key, value);
            } catch (string &e) {
                throw e;
            }
        }
    }
    delete bucket;
    return result;
}

vector<string> ChainedDirectory::getAllValues()
{
    vector<string> elements;
    DepthBucket* bucket;
    for(vector<string>::iterator it = buckets.begin(); it != buckets.end(); ++it) {
        bucket = factory->readBucket(*it);
        vector<string>& values = bucket->getAllValues();
        elements.insert(elements.end(), values.begin(), values.end());
    }

    if (nextDirectory) {
        vector<string> values = nextDirectory->getAllValues();
        elements.insert(elements.end(), values.begin(), values.end());
    }

    delete bucket;
    return elements;
}

void ChainedDirectory::putValue(size_t key, string value)
{
    DepthBucket *bucket = getBucket(key);

    if (!bucket->isFull()) {
        bucket->putValue(value);
        factory->writeBucket(bucket);
    } else {
        if (canBeDoubled() || bucket->getLocalDepth() < globalDepth) {

            if (bucket->getLocalDepth() == globalDepth && canBeDoubled()) {
                doubleSize();
                numberDoubling += 1;
            }

            if (bucket->getLocalDepth() < globalDepth) {
                split(bucket);
                bucket = getBucket(key);
                bucket->putValue(value);
                factory->writeBucket(bucket);
            }

        } else {
            if (!nextDirectory)
                nextDirectory = new ChainedDirectory(hasher);
            nextDirectory->putValue(key, value);
        }
    }
    delete bucket;
}

void ChainedDirectory::split(DepthBucket* bucket)
{
    Directory::split(bucket);
    numberBuckets++;
    notifyNumberBuckets(1);
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

string ChainedDirectory::className() const
{
    return "ChainedDirectory ";
}

void ChainedDirectory::notifyNumberBuckets(int number)
{
    hasher->setNumberBuckets(hasher->getNumberBuckets() + number);
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
