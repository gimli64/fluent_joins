#include "bucket.h"
#include "depthBucket.h"
#include "chainedBucket.h"
#define BOOST_TEST_MODULE bucketTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(bucket_test)
{
    Bucket bucket = Bucket();
    bucket.putValue("0");
    BOOST_CHECK_EQUAL("0", bucket.getValue("0"));
    try {
        bucket.getValue("1");
    } catch (string &e) {
//        BOOST_CHECK_EQUAL("Value wasn't found", e);
    }

    BOOST_CHECK_EQUAL(false, bucket.isFull());
    BOOST_CHECK_EQUAL(1, bucket.size());

    bucket = Bucket();
    for (int i = 0; i < Bucket::BUCKET_SIZE; i++) {
        bucket.putValue("0");
    }
    BOOST_CHECK_EQUAL(true, bucket.isFull());
    BOOST_CHECK_EQUAL("0", bucket.getAllValues()[0]);
}

BOOST_AUTO_TEST_CASE(depthBucket_test)
{
    DepthBucket bucket = DepthBucket();
    BOOST_CHECK_EQUAL(0, bucket.getLocalDepth());
    bucket.setLocalDepth(1);
    BOOST_CHECK_EQUAL(1, bucket.getLocalDepth());
}

//BOOST_AUTO_TEST_CASE(chainedBucket_test)
//{
//    ChainedBucket bucket = ChainedBucket();
//    for (int i = 0; i < Bucket::BUCKET_SIZE; i++) {
//        bucket.putValue("0");
//    }
//    BOOST_CHECK_EQUAL(true, bucket.isFull());
//    BOOST_CHECK_EQUAL(1, bucket.getChainCount());
//    bucket.putValue("1");
//    BOOST_CHECK_EQUAL("1", bucket.getValue("1"));
//    BOOST_CHECK_EQUAL(2, bucket.getChainCount());
//}
