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

    T* newBucket();                 // Creation without serialization
    void deleteBucket(T *bucket);   // Delete on memory change number buckets

    void reset();                   // Reset the factory variables for new initialization

    int getBucketCount();
    int getNumberBuckets();

    void printState();

private:
    BucketFactory();
    static BucketFactory<T>* instance;
    int bucketCount;           // Used as a postfix for the bucket names
    int numberBuckets;         // Actual number of buckets (for building phase)
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
    :bucketCount(0), numberBuckets(0)
{
}

template<class T>
T* BucketFactory<T>::newBucket()
{
    T *bucket = new T(lexical_cast<string>(bucketCount));
    bucketCount++;
    numberBuckets++;
    return bucket;
}

template<class T>
void BucketFactory<T>::deleteBucket(T *bucket)
{
    numberBuckets -=  1;
    delete bucket;
}

template<class T>
void BucketFactory<T>::reset()
{
    numberBuckets = 0;
    bucketCount = 0;
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
void BucketFactory<T>::printState()
{
    cout << "number buckets : " << numberBuckets << endl;
}

#endif // BUCKETFACTORY_H
