#include "extendible/extendibleHashing.h"
#define BOOST_TEST_MODULE hashingMethodTests
#include <boost/test/unit_test.hpp>

//BOOST_AUTO_TEST_CASE(hashingMethod_test)
//{
//    HashingMethod hasher = HashingMethod();
//    vector<string> values;
//    values.push_back("0");
//    values.push_back("1");
//    hasher.put(Couple("0", values));
//    try {
//        hasher.get("0");
//    } catch (string &e) {
//        BOOST_CHECK_EQUAL("Value wasn't found", e);
//    }
//}

//BOOST_AUTO_TEST_CASE(extendibleHashing_test)
//{
//    ExtendibleHashing hasher = ExtendibleHashing();
//    vector<string> values;
//    values.push_back("0");
//    values.push_back("1");
//    hasher.put(Couple("0", values));
//    BucketFactory<DepthBucket>::getInstance()->writeAll(hasher.getBuckets(), "test");
//    BOOST_CHECK_EQUAL("1", hasher.get("0").at(1));
//    try {
//        hasher.get("1");
//    } catch (string &e) {
//        BOOST_CHECK_EQUAL("Value wasn't found", e);
//    }
//}
