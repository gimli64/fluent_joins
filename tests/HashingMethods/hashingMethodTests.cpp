#include "common/hashingMethod.h"
#include "extendible/extendibleHashing.h"
#include "linear/linearHashing.h"
#include "hybrid/hybridHashing.h"
#define BOOST_TEST_MODULE hashingMethodTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(hashingMethod_test)
{
    HashingMethod *hasher = HashingMethod::getInstance();
    hasher->put(Couple("0", "0"));
    try {
        hasher->get("0");
    } catch (string &e) {
        BOOST_CHECK_EQUAL("Value wasn't found", e);
    }
}

BOOST_AUTO_TEST_CASE(extendibleHashing_test)
{
    ExtendibleHashing hasher = ExtendibleHashing();
    hasher.put(Couple("0", "0"));
    BOOST_CHECK_EQUAL("0", hasher.get("0"));
    try {
        hasher.get("1");
    } catch (string &e) {
        BOOST_CHECK_EQUAL("Value wasn't found", e);
    }
}

BOOST_AUTO_TEST_CASE(linearHashing_test)
{
    LinearHashing hasher = LinearHashing();
    hasher.put(Couple("0", "0"));
    BOOST_CHECK_EQUAL("0", hasher.get("0"));
    try {
        hasher.get("1");
    } catch (string &e) {
        BOOST_CHECK_EQUAL("Value wasn't found", e);
    }

}

BOOST_AUTO_TEST_CASE(hybridHashing_test)
{
    HybridHashing hasher = HybridHashing();
    hasher.put(Couple("0", "0"));
    BOOST_CHECK_EQUAL("0", hasher.get("0"));
    try {
        hasher.get("1");
    } catch (string &e) {
        BOOST_CHECK_EQUAL("Value wasn't found", e);
    };
}
