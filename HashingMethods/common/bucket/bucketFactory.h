#ifndef BUCKETFACTORY_H
#define BUCKETFACTORY_H

#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

using namespace std;
using namespace boost;
using boost::archive::text_iarchive;
using boost::archive::text_oarchive;

template<class T> class BucketFactory
{
public:
    static BucketFactory<T>* getInstance();

    T* readBucket(string bucketFile) const;
    void writeBucket(T *bucket, string bucketPath);

    T* newBucket();                 // Creation without serialization
    void deleteBucket(T *bucket);   // Delete on memory change number buckets
    void removeBucket(T *bucket);   // Remove on disk and change number buckets

    void reset();                   // Reset the factory variables for new initialization

    // Used at the end of a hash table initialization, serialize all its buckets
    // and delete the memory objects
    void writeAll(vector<T*> buckets, string bucketPath);

    // Remove all buckets from disk
    void removeAll(string bucketPath);

    int getBucketCount();
    int getNumberBuckets();

    void incChainNumber(int chainCount);
    double getOverflowRatio();
    void printState();

private:
    BucketFactory();
    static BucketFactory<T>* instance;
    int bucketCount;           // Used as a postfix for the bucket names
    int numberBuckets;         // Actual number of buckets (for building phase)
    const string constPrefix;  // Beginning of path to the buckets
    const string bucketPrefix;

    int maxChainLength;
    int numberOverflowBuckets;
    int numberLongChain;
    int numberChain;
};


template<class T>
BucketFactory<T>* BucketFactory<T>::instance = 0;

template<class T>
BucketFactory<T>* BucketFactory<T>::getInstance()
{
    if (instance == 0)
        instance = new BucketFactory<T>();
    return instance;
}

template<class T>
BucketFactory<T>::BucketFactory()
    :bucketCount(0), numberBuckets(0), constPrefix("/tmp/buckets/"), bucketPrefix("b"),
      maxChainLength(0), numberOverflowBuckets(0), numberChain(0), numberLongChain(0)
{
}

template<class T>
T* BucketFactory<T>::readBucket(string bucketFile) const
{
    T* bucket = new T();
    {
        ifstream ifs((constPrefix + bucketFile).c_str());
        text_iarchive ia(ifs);
        ia >> *bucket;
    }
    return bucket;
}

template<class T>
void BucketFactory<T>::writeBucket(T *bucket, string bucketPath)
{
    ofstream ofs((constPrefix + bucketPath + bucket->name).c_str());
    {
        text_oarchive oa(ofs);
        oa << *bucket;
    }
}

template<class T>
T* BucketFactory<T>::newBucket()
{
    T *bucket = new T(bucketPrefix + lexical_cast<string>(bucketCount));
    bucketCount++;
    numberBuckets++;
    return bucket;
}

template<class T>
void BucketFactory<T>::deleteBucket(T *bucket)
{
    int chainCount = bucket->getChainCount();
    numberBuckets -= chainCount;
    chainCount -= 1;
    if (chainCount >= 1) {
        numberOverflowBuckets -= chainCount;
        numberChain -= 1;
    }
    if (chainCount == maxChainLength) {
        maxChainLength -= 1;
    }
    if (chainCount > 1) {
        numberLongChain -= 1;
    }

    delete bucket;
}

template<class T>
void BucketFactory<T>::removeBucket(T *bucket)
{
    numberBuckets -= bucket->getChainCount();
    remove(bucket->name.c_str());
    delete bucket;
}

template<class T>
void BucketFactory<T>::reset()
{
    numberBuckets = 0;
    bucketCount = 0;
}

template<class T>
void BucketFactory<T>::removeAll(string bucketPath)
{
    reset();
    system(("exec find " + constPrefix + bucketPath + " -name '" + bucketPrefix + "*' | xargs rm").c_str());
    system(("exec find " + constPrefix + bucketPath + "_aux" + " -name '" + bucketPrefix + "*' | xargs rm").c_str());
}

template<class T>
void BucketFactory<T>::writeAll(vector<T*> buckets, string bucketPath)
{
    for(typename vector<T*>::iterator it = buckets.begin(); it != buckets.end(); ++it) {
        T* bucket = *it;
        writeBucket(bucket, bucketPath);

        while(bucket->hasNext()) {
            bucket = bucket->next();
            writeBucket(bucket, bucketPath);
        }
    }
}

template<class T>
int BucketFactory<T>::getBucketCount()
{
    return bucketCount;
}

template<class T>
int BucketFactory<T>::getNumberBuckets()
{
    return numberBuckets;
}

template<class T>
void BucketFactory<T>::incChainNumber(int chainCount)
{
    numberOverflowBuckets += 1;
    if (chainCount == 1)
        numberChain += 1;

    if (chainCount > maxChainLength)
        maxChainLength += 1;

    if (chainCount > 1)
        numberLongChain += 1;
}

template<class T>
void BucketFactory<T>::printState()
{
    cout << "bucketFactory numbers" << endl;
    cout << "max chain length : " << maxChainLength << endl;
    cout << "number overflow buckets : " << numberOverflowBuckets << endl;
    cout << "number long chain : " << numberLongChain << endl;
    cout << "number chain : " << numberChain << endl;
}

template<class T>
double BucketFactory<T>::getOverflowRatio()
{
    return (double) numberOverflowBuckets / numberBuckets;
}

#endif // BUCKETFACTORY_H
