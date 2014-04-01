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
    HashingMethod hasher = HashingMethod();
    Directory directory = Directory(&hasher);
    BOOST_CHECK_EQUAL(0, directory.getGlobalDepth());
    size_t key = hasher.getHash("0");
    vector<string> values;
    values.push_back("0");
    values.push_back("1");
    directory.putCouple(key, Couple("0", values));
    BucketFactory<DepthBucket>::getInstance()->writeAll(directory.getBuckets(), "test");
    BOOST_CHECK_EQUAL("1", directory.getValue(key, "0").at(1));

    for (int i = 1; i < Bucket::BUCKET_SIZE; i++) {
        directory.putCouple(key, Couple("0", values));
    }
    BOOST_CHECK_EQUAL(0, directory.getGlobalDepth());
    directory.putCouple(key, Couple("0", values));
    BOOST_CHECK_EQUAL(1, directory.getGlobalDepth());
}

BOOST_AUTO_TEST_CASE(HybridDirectory_test)
{
    HashingMethod hasher = HashingMethod();
    HybridDirectory directory = HybridDirectory(&hasher);
    size_t key = hasher.getHash("0");
    vector<string> values;
    values.push_back("0");
    values.push_back("1");
    for (int i = 0; i < Bucket::BUCKET_SIZE; i++) {
        directory.putCouple(key, Couple("0", values));
    }

    BucketFactory<DepthBucket>::getInstance()->writeAll(directory.getBuckets(), "test");
    BOOST_CHECK_EQUAL("1", directory.getValue(key, "0").at(1));
    // Todo, reimplement the whole test
}
