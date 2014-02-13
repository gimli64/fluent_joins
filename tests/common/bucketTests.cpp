#include "bucket.h"
#define BOOST_TEST_MODULE bucketTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(bucket_test)
{
    Bucket bucket = Bucket();
    bucket.putValue("0");
    BOOST_CHECK_EQUAL("0", bucket.getValue("0"));
    BOOST_CHECK_EQUAL(false, bucket.isFull());
    BOOST_CHECK_EQUAL(1, bucket.size());
}
