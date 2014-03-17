#include "directory.h"
#include "hashingMethod.h"
#define BOOST_TEST_MODULE directoryTests
#include <boost/test/unit_test.hpp>
#include <stdlib.h>

BOOST_AUTO_TEST_CASE(directory_test)
{
    HashingMethod hasher = HashingMethod();
    Directory directory = Directory(hasher);
    BOOST_CHECK_EQUAL(1, directory.getBuckets().size());
    BOOST_CHECK_EQUAL(0, directory.getGlobalDepth());
    size_t key = hasher.getHash("0");
    directory.putValue(key, "0");
    BOOST_CHECK_EQUAL("0", *(directory.getValue(key, "0")));

    for (int i = 1; i < Bucket::BUCKET_SIZE; i++) {
        directory.putValue(key, "0");
    }
    BOOST_CHECK_EQUAL(1, directory.getBuckets().size());
    BOOST_CHECK_EQUAL(0, directory.getGlobalDepth());
    directory.putValue(key, "0");
//    BOOST_CHECK_EQUAL(2, directory.getBuckets().size());
//    BOOST_CHECK_EQUAL(1, directory.getGlobalDepth());
}

