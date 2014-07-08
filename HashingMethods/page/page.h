#ifndef PAGE_H
#define PAGE_H

#include <vector>

#include "common/couple.h"

using namespace std;

class Page
{
public:

    Page();

    virtual vector<string> getValue(string key);
    virtual void putCouple(Couple couple);

    int getDepth();
    void setDepth(int depth);

private:
    int depth;
};

#endif // PAGE_H
