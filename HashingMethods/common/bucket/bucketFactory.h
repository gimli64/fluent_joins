#ifndef BUCKETFACTORY_H
#define BUCKETFACTORY_H

#include <fstream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace boost;
using boost::archive::text_iarchive;
using boost::archive::text_oarchive;

template<class T> class BucketFactory
{
public:
    static BucketFactory<T>* getInstance();

    T* readBucket(string bucketFile) const;
    void writeBucket(T *bucket);

    T* newBucket();     // Creation without serialization
    T* createBucket();  // Creation and serialization
    void deleteBucket(T *bucket);   // Remove and change number buckets
    void removeBucket(T *bucket);   // Remove without changing number buckets

    int getBucketCount();

private:
    BucketFactory();
    static BucketFactory<T>* instance;
    int bucketCount;
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
    :bucketCount(0)
{
}

template<class T>
T* BucketFactory<T>::readBucket(string bucketFile) const
{
    T* bucket = new T();
    {
        ifstream ifs(bucketFile.c_str());
        text_iarchive ia(ifs);
        ia >> *bucket;
    }
    return bucket;
}

template<class T>
void BucketFactory<T>::writeBucket(T *bucket)
{
    ofstream ofs(bucket->name.c_str());
    {
        text_oarchive oa(ofs);
        oa << *bucket;
    }
}

template<class T>
T* BucketFactory<T>::newBucket()
{
    T *bucket = new T();
    bucket->name += lexical_cast<string>(bucketCount);
    bucketCount++;
    return bucket;
}

template<class T>
T* BucketFactory<T>::createBucket()
{
    T *bucket = newBucket();
    writeBucket(bucket);
    return bucket;
}

template<class T>
void BucketFactory<T>::deleteBucket(T *bucket)
{
    bucketCount -= bucket->getChainCount();
    removeBucket(bucket);
}

template<class T>
void BucketFactory<T>::removeBucket(T *bucket)
{
    remove(bucket->name.c_str());
    delete bucket;
}

template<class T>
int BucketFactory<T>::getBucketCount()
{
    return bucketCount;
}
#endif // BUCKETFACTORY_H
