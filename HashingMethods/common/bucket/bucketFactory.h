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
    void writeBucket(T *bucket);

    T* newBucket();     // Creation without serialization
    void deleteBucket(T *bucket);   // Delete on memory change number buckets
    void removeBucket(T *bucket);   // Remove on disk and change number buckets

    // Used at the end of a hash table initialization, serialize all its buckets
    // and delete the memory objects
    void writeAll(vector<T*> buckets);
    void removeAll();

    int getBucketCount();
    int getNumberBuckets();
    void setNumberBuckets(int number);

private:
    BucketFactory();
    static BucketFactory<T>* instance;
    int bucketCount;
    int numberBuckets;
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
    numberBuckets++;
    return bucket;
}

template<class T>
void BucketFactory<T>::deleteBucket(T *bucket)
{
    numberBuckets -= bucket->getChainCount();
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
void BucketFactory<T>::removeAll()
{
    numberBuckets = 0;
    bucketCount = 0;
    system("exec rm /tmp/buckets/*");
}

template<class T>
void BucketFactory<T>::writeAll(vector<T*> buckets)
{
    for(typename vector<T*>::iterator it = buckets.begin(); it != buckets.end(); ++it) {
        writeBucket(*it);
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
void BucketFactory<T>::setNumberBuckets(int number)
{
    numberBuckets = number;
}
#endif // BUCKETFACTORY_H
