#ifndef MULTIKEYHYBRIDHASHING_H
#define MULTIKEYHYBRIDHASHING_H
#include "common/hashing/multikeyHashTable.h"
#include "hybridHashing.h"
#include "common/directory/hybridDirectory.h"
#include <boost/serialization/string.hpp>

using namespace std;

class MultikeyHybridHashing : public HybridHashing
{
public:
    MultikeyHybridHashing(string name = "", vector<int> keysRepartition = vector<int>());

    vector<Couple> getCouples();

private:
    virtual Bucket *fetchBucket(size_t hash);

    virtual HybridDirectory& getHybridDirectory(size_t hash);
    virtual size_t getLeftMostBits(size_t hash);
    virtual void incrementSplitIndex();
    virtual void split();

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<HybridHashing>(*this);
    }
};

ostream& operator<<(ostream&, const MultikeyHybridHashing&);

#endif // HYBRIDHASHING_H
