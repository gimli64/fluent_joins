#include "common/bucket/chainedBucket.h"
#include "common/bucket/depthBucket.h"
#include "common/directory/directory.h"
#include "common/hashingMethod.h"
#include "linear/linearHashing.h"

using namespace std;

int main()
{
    Directory dir = Directory();
    dir.init();
    cout << dir << endl;
}
