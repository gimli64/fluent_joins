#include "hashing/hashTable.h"
#include "page/directory.h"
#define BOOST_TEST_MODULE directoryTests
#include <boost/test/unit_test.hpp>
#include <stdlib.h>
#include <math.h>

using std::cout;
using std::endl;

BOOST_AUTO_TEST_CASE(directory_test)
{
    vector<int> keysRepartition;
    keysRepartition.push_back(0);
    HashTable hasher = HashTable("test", keysRepartition);
    Directory directory = Directory(&hasher);
    size_t key = hasher.getHash("0");
    cout << key << endl;
    vector<string> values;
    values.push_back("0");
    values.push_back("1");
    directory.putCouple(key, Couple("0", values));
    BOOST_CHECK_EQUAL("1", directory.getValue(key, "0").at(1));

    for (int i = 1; i < Bucket::BUCKET_SIZE; i++) {
        directory.putCouple(key, Couple("0", values));
    }
    BOOST_CHECK_EQUAL(0, directory.getDepth());
    key = hasher.getHash("1");
    cout << key << endl;
    directory.putCouple(key, Couple("1", values));
//    BOOST_CHECK_EQUAL(1, directory.getDepth());
}
