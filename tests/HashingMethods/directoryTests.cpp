#include "common/directory/directory.h"
#include "common/directory/hybridDirectory.h"
#include "common/hashingMethod.h"
#define BOOST_TEST_MODULE directoryTests
#include <boost/test/unit_test.hpp>
#include <stdlib.h>
#include <math.h>

using std::cout;
using std::endl;

BOOST_AUTO_TEST_CASE(directory_test)
{
    HashingMethod *hasher = HashingMethod::getInstance();
    Directory directory;
    BOOST_CHECK_EQUAL(0, directory.getGlobalDepth());
    size_t key = hasher->getHash("0");
    directory.putCouple(key, Couple("0", "0"));
    BOOST_CHECK_EQUAL("0", directory.getValue(key, "0"));

    for (int i = 1; i < Bucket::BUCKET_SIZE; i++) {
        directory.putCouple(key, Couple("0", "0"));
    }
    BOOST_CHECK_EQUAL(0, directory.getGlobalDepth());
    directory.putCouple(key, Couple("0", "0"));
    BOOST_CHECK_EQUAL(1, directory.getGlobalDepth());
}

BOOST_AUTO_TEST_CASE(HybridDirectory_test)
{
    HashingMethod *hasher = HashingMethod::getInstance();
    HybridDirectory directory = HybridDirectory(hasher);
    size_t key = hasher->getHash("0");
    for (int i = 0; i < Bucket::BUCKET_SIZE; i++) {
        directory.putCouple(key, Couple("0", "0"));
    }

    BOOST_CHECK_EQUAL("0", directory.getValue(key, "0"));
    // Todo, reimplement the whole test
}
