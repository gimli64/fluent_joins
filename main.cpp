#include "common/bucket/chainedBucket.h"
#include "common/bucket/depthBucket.h"
#include "common/directory/directory.h"
#include "common/directory/hybridDirectory.h"
#include "common/hashingMethod.h"
#include "linear/linearHashing.h"
#include "extendible/extendibleHashing.h"
#include "hybrid/hybridHashing.h"

#include "common/structure/couple.h"

using namespace std;

int main()
{
    LinearHashing hasher = LinearHashing();
    vector<string> values;
    values.push_back("0");
    values.push_back("1");
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
    hasher.put(Couple("0", values));
    cout << hasher << endl;
}
