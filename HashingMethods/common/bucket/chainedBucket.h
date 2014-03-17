#ifndef CHAINEDBUCKET_H
#define CHAINEDBUCKET_H
#include "bucket.h"

class ChainedBucket : public Bucket
{
public:
    ChainedBucket();
    ChainedBucket(HashingMethod& hasher);

    ~ChainedBucket();

    string* getValue(string value);
    void putValue(string value);
    int getChainCount();
    vector<string> getAllValues();

    virtual ostream& dump(ostream &strm) const {
        ostream& output = Bucket::dump(strm);
        if (nextBucket) {
            output << " --> ";
            return nextBucket->dump(output);
        }
        return output;
    }

private:
    ChainedBucket* nextBucket;
};

#endif // CHAINEDBUCKET_H
