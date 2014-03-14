#include "directory.h"
#include "hashingMethod.h"
#define BOOST_TEST_MODULE bucketTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(directory_test)
{
    HashingMethod hasher = HashingMethod();
    Directory directory = Directory(hasher);
    size_t key = hasher.getHash("0");
    directory.putValue(key, "0");
    BOOST_CHECK_EQUAL("0", *(directory.getValue(key, "0")));
}

