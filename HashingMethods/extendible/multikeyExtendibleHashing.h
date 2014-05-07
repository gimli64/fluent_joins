#ifndef MULTIKEYEXTENDIBLEHASHING_H
#define MULTIKEYEXTENDIBLEHASHING_H

#include "extendibleHashing.h"

class MultikeyExtendibleHashing : public ExtendibleHashing
{
public:
    MultikeyExtendibleHashing(string name = "", vector<int> keysRepartition = vector<int>());

    virtual void checkStructure();
    double updateStructure();
    void reInsertCouples();

private:
    int maxChainLength;
    int numberOverflowBuckets;
    int numberLongChain;
    int numberChain;
    double loadFactor;
    vector<Couple> insertedCouples;

    virtual Bucket *fetchBucket(size_t hash);

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<ExtendibleHashing>(*this);
    }
};

ostream& operator<<(ostream&, const MultikeyExtendibleHashing&);
#endif // MULTIKEYEXTENDIBLEHASHING_H
