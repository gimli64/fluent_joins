#ifndef PAGE_H
#define PAGE_H

#include <vector>

#include "common/couple.h"

using namespace std;

class Page
{
public:

    Page();
    int getDepth();
    void setDepth(int depth);

    virtual bool isBucket() = 0;
    virtual Page* getBucket(size_t hash) = 0;

protected:
    int depth;
};

#endif // PAGE_H
