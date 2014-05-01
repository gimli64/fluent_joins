#ifndef MULTIKEYLINEARHASHING_H
#define MULTIKEYLINEARHASHING_H

#include "linearHashing.h"

class MultikeyLinearHashing : public LinearHashing
{
public:
    MultikeyLinearHashing(string name = "", vector<int> keysRepartition = vector<int>());

    vector<Couple> getCouples();
    virtual void printState();

private:
    virtual Bucket *fetchBucket(size_t hash);
    virtual void split();

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<LinearHashing>(*this);
    }
};

#endif // MULTIKEYLINEARHASHING_H
