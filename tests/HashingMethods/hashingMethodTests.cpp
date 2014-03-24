#include "common/hashingMethod.h"
#include "extendible/extendibleHashing.h"
#include "linear/linearHashing.h"
#include "hybrid/hybridHashing.h"
#define BOOST_TEST_MODULE hashingMethodTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(hashingMethod_test)
{
    HashingMethod *hasher = HashingMethod::getInstance();
    hasher->put("0");
    try {
        hasher->get("0");
    } catch (string &e) {
        BOOST_CHECK_EQUAL("Value wasn't found", e);
    }
}

BOOST_AUTO_TEST_CASE(extendibleHashing_test)
{
    ExtendibleHashing *hasher = ExtendibleHashing::getInstance();
    hasher->put("0");
    BOOST_CHECK_EQUAL("0", hasher->get("0"));
    try {
        hasher->get("1");
    } catch (string &e) {
        BOOST_CHECK_EQUAL("Value wasn't found", e);
    }
}

//BOOST_AUTO_TEST_CASE(linearHashing_test)
//{
//    LinearHashing hasher = LinearHashing();
//    cout << hasher.getNumberBuckets() << endl;
//    hasher.put("0");
//    BOOST_CHECK_EQUAL("0", hasher.get("0"));
//    try {
//        hasher.get("1");
//    } catch (string &e) {
////        BOOST_CHECK_EQUAL("Value wasn't found", e);
//    }
//    for (int i = 0; i < 10; i++) {
//        hasher.put("0");
////        cout << hasher << endl;
//    }

////    hasher.put("1");
////    cout << hasher << endl;
////    hasher.put("2");
////    cout << hasher << endl;
////    hasher.put("3");
////    cout << hasher << endl;
////    hasher.put("4");
////    cout << hasher << endl;
////    hasher.put("5");
////    cout << hasher << endl;
////    hasher.put("6");
////    cout << hasher << endl;
////    hasher.put("7");
////    cout << hasher << endl;
////    hasher.put("8");
////    cout << hasher << endl;
////    hasher.put("9");
////    cout << hasher << endl;
////    hasher.put("10");
////    cout << hasher << endl;
////    hasher.put("11");
////    cout << hasher << endl;
//}

//BOOST_AUTO_TEST_CASE(hybridHashing_test)
//{
//    HybridHashing hasher = HybridHashing();
//    hasher.put("0");
//    BOOST_CHECK_EQUAL("0", hasher.get("0"));
//    try {
//        hasher.get("1");
//    } catch (string &e) {
//        BOOST_CHECK_EQUAL("Value wasn't found", e);
//    };
//}
