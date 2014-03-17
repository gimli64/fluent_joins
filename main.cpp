#include "hashingMethod.h"
#include "directory.h"
#include "chainedDirectory.h"
#include "Bucket.h"

using std::cout;
using std::endl;

int main()
{
    HashingMethod hasher = HashingMethod();
    ChainedDirectory directory = ChainedDirectory(hasher);
    cout << directory << endl;
}
