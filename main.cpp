#include "hashingMethod.h"
#include "directory.h"
using std::cout;
using std::endl;

int main()
{
    vector<int> a = vector<int>();
    a.push_back(0);
    a.push_back(1);
    int y = 4;
    a.at(0) = y;
    y = 5;
    cout << a.at(0) << endl;
}
