#ifndef PAGE_H
#define PAGE_H

#include <vector>

#include "common/couple.h"

using namespace std;

class Page
{
public:
    static const int PAGE_SIZE = 4096;

    Page();
    int getDepth();
    void setDepth(int depth);

    virtual vector<string> getValue(size_t hash) = 0;
    virtual void putCouple(size_t hash, Couple couple) = 0;

    virtual Page* getBucket(size_t hash) = 0;
    virtual bool isBucket();\

    virtual string className() const;
    virtual ostream& dump(ostream& strm) const;

protected:
    int depth;
};

ostream& operator<<(ostream&, const Page&);

#endif // PAGE_H
