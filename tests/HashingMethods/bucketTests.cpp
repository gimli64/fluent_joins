#include "common/bucket/bucket.h"
#include "common/bucket/depthBucket.h"
#include "common/bucket/chainedBucket.h"
#define BOOST_TEST_MODULE bucketTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(bucket_test)
{
    Bucket bucket = Bucket();
    vector<string> values;
    values.push_back("0");
    values.push_back("1");
    bucket.putCouple(Couple("0", values));
    BOOST_CHECK_EQUAL("1", bucket.getValue("0").at(1));
    try {
        bucket.getValue("1");
    } catch (string &e) {
        BOOST_CHECK_EQUAL("Value wasn't found", e);
    }

    BOOST_CHECK_EQUAL(false, bucket.isFull());
    BOOST_CHECK_EQUAL(1, bucket.size());

    bucket = Bucket();
    for (int i = 0; i < Bucket::BUCKET_SIZE; i++) {
        bucket.putCouple(Couple("0", values));
    }
    BOOST_CHECK_EQUAL(true, bucket.isFull());
//    BOOST_CHECK_EQUAL(, bucket.getAllValues()[0]);
}

BOOST_AUTO_TEST_CASE(depthBucket_test)
{
    DepthBucket bucket = DepthBucket();
    BOOST_CHECK_EQUAL(0, bucket.getLocalDepth());
    bucket.setLocalDepth(1);
    BOOST_CHECK_EQUAL(1, bucket.getLocalDepth());
}

BOOST_AUTO_TEST_CASE(chainedBucket_test)
{
    ChainedBucket bucket = ChainedBucket();
    vector<string> values;
    values.push_back("0");
    values.push_back("1");
    for (int i = 0; i < Bucket::BUCKET_SIZE; i++) {
        bucket.putCouple(Couple("0", values));
    }
    BOOST_CHECK_EQUAL(true, bucket.isFull());
    BOOST_CHECK_EQUAL(1, bucket.getChainCount());
    bucket.putCouple(Couple("1", values));
    BOOST_CHECK_EQUAL("1", bucket.getValue("1").at(1));
    BOOST_CHECK_EQUAL(2, bucket.getChainCount());
}
