#include "hashingMethod.h"
#include "extendibleHashing.h"
#include "linearHashing.h"
#include "hybridHashing.h"
#define BOOST_TEST_MODULE hashingMethodTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(hashingMethod_test)
{
    HashingMethod hasher = HashingMethod();
    hasher.put("0");
    BOOST_CHECK_EQUAL((string*) NULL, hasher.get("0"));
}

BOOST_AUTO_TEST_CASE(extendibleHashing_test)
{
    ExtendibleHashing hasher = ExtendibleHashing();
    hasher.put("0");
    BOOST_CHECK_EQUAL("0", *hasher.get("0"));
    BOOST_CHECK_EQUAL((string*) NULL, hasher.get("1"));
}

BOOST_AUTO_TEST_CASE(linearHashing_test)
{
    LinearHashing hasher = LinearHashing();
    cout << hasher.getNumberBuckets() << endl;
    hasher.put("0");
    BOOST_CHECK_EQUAL("0", *hasher.get("0"));
    BOOST_CHECK_EQUAL((string*) NULL, hasher.get("1"));
    for (int i = 0; i < 10; i++) {
        hasher.put("0");
        cout << hasher << endl;
    }

    hasher.put("1");
    cout << hasher << endl;
    hasher.put("2");
    cout << hasher << endl;
    hasher.put("3");
    cout << hasher << endl;
    hasher.put("4");
    cout << hasher << endl;
    hasher.put("5");
    cout << hasher << endl;
    hasher.put("6");
    cout << hasher << endl;
    hasher.put("7");
    cout << hasher << endl;
    hasher.put("8");
    cout << hasher << endl;
    hasher.put("9");
    cout << hasher << endl;
    hasher.put("10");
    cout << hasher << endl;
    hasher.put("11");
    cout << hasher << endl;
}

BOOST_AUTO_TEST_CASE(hybridHashing_test)
{
    HybridHashing hasher = HybridHashing();
    hasher.put("0");
    BOOST_CHECK_EQUAL("0", *hasher.get("0"));
    BOOST_CHECK_EQUAL((string*) NULL, hasher.get("1"));
}
