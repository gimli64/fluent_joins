#include "chainedBucket.h"
#include "depthBucket.h"
#include "directory.h"
#include "hashingMethod.h"

int main()
{
    HashingMethod hasher;
    Directory dir(&hasher);
    size_t key = hasher.getHash("0");
    cout << dir << endl;
    dir.putValue(key, "0");
    dir.putValue(key, "0");
    dir.putValue(key, "0");
    dir.putValue(key, "0");
    dir.putValue(key, "0");
    dir.putValue(key, "0");
    dir.putValue(key, "0");
    dir.putValue(key, "0");
    dir.putValue(key, "0");
    dir.putValue(key, "0");
    dir.putValue(key, "0");
    cout << dir << endl;
    return 0;
}
