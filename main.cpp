#include "common/bucket/chainedBucket.h"
#include "common/bucket/depthBucket.h"
#include "common/directory/directory.h"
#include "common/hashingMethod.h"
#include "linear/linearHashing.h"

using namespace std;

int main()
{
    LinearHashing *hasher = LinearHashing::getInstance();
    cout << *hasher << endl;
    hasher->put("0");
    cout << *hasher << endl;
    hasher->put("0");
    cout << *hasher << endl;
    hasher->put("0");
    cout << *hasher << endl;
    hasher->put("0");
    cout << *hasher << endl;
    hasher->put("0");
    cout << *hasher << endl;
    hasher->put("0");
    cout << *hasher << endl;
    hasher->put("0");
    cout << *hasher << endl;
    hasher->put("0");
    cout << *hasher << endl;
    hasher->put("0");
    cout << *hasher << endl;
    hasher->put("0");
    cout << *hasher << endl;
    hasher->put("0");
    cout << *hasher << endl;
}
