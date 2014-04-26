#include "multikeyLinearHashing.h"

MultikeyLinearHashing::MultikeyLinearHashing(string name, vector<int> keysRepartition)
    :LinearHashing(name, keysRepartition)
{
}

vector<Couple> MultikeyLinearHashing::getCouples()
{
    vector<Couple> couples;
    string bucketName;
    ChainedBucket *bucket;
    for (int i = 0; i < bucketNames.size(); i++) {
        bucketName = bucketNames.at(i);
        bucket = factory->readBucket(bucketPath + bucketNames.at(i));
        numberBucketFetch++;
        couples.insert(couples.end(), bucket->elements.begin(), bucket->elements.end());
        while (bucket->hasNext()) {
            bucketName = bucket->nextName();
            bucket = factory->readBucket(bucketPath + bucketName);
            numberBucketFetch++;
            couples.insert(couples.end(), bucket->elements.begin(), bucket->elements.end());
        }
    }

    return couples;
}

Bucket *MultikeyLinearHashing::fetchBucket(size_t hash)
{
    numberBucketFetch++;
    return getBucketFromName(hash);
}

void MultikeyLinearHashing::split()
{
    ChainedBucket *bucketToSplit = buckets.at(nextSplitIndex);
    vector<Couple> values = bucketToSplit->elements;

    ChainedBucket *nextBucket = bucketToSplit;
    while (nextBucket->hasNext()) {
        nextBucket = nextBucket->next();
        values.insert(values.end(), nextBucket->elements.begin(), nextBucket->elements.end());
    }

    ChainedBucket *newBucket1 = factory->newBucket();
    newBucket1->setBucketPath(bucketPath);
    bucketNames.at(nextSplitIndex) = newBucket1->name;
    buckets.at(nextSplitIndex) = newBucket1;

    ChainedBucket *newBucket2 = factory->newBucket();
    newBucket2->setBucketPath(bucketPath);
    buckets.push_back(newBucket2);
    bucketNames.push_back(newBucket2->name);

    numberItems -= values.size();
    incrementSplitIndex();
    factory->deleteBucket(bucketToSplit);

    for (vector<Couple>::iterator it = values.begin(); it != values.end(); ++it) {
        putMultikey(*it);
    }
}
