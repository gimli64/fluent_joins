#include "common/bucket/chainedBucket.h"
#include "common/bucket/depthBucket.h"
#include "common/directory/directory.h"
#include "common/directory/chainedDirectory.h"
#include "common/hashingMethod.h"
#include "linear/linearHashing.h"

using namespace std;

int main()
{
    HashingMethod hasher = HashingMethod();
    ChainedDirectory dir = ChainedDirectory(&hasher);
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
    dir.putValue(0, "0");
    cout << dir << endl;
}
