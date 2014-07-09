#include "page.h"

Page::Page()
    :depth(0)
{
}

void Page::putCouple(size_t hash, Couple couple) {}

int Page::getDepth()
{
    return depth;
}

void Page::setDepth(int depth)
{
    this->depth = depth;
}

bool Page::isBucket()
{
    return false;
}

bool Page::isFull()
{
    return false;
}

void Page::doubleSize() {}

int Page::getGlobalDepth()
{
    return 0;
}

string Page::className() const
{
    return "Page ";
}

ostream& Page::dump(ostream& strm) const
{
    const void * address = static_cast<const void*>(this);
    stringstream ss;
    ss << address;
    ostream& output = strm;
    output << className() + ss.str() + ", depth ";
    output << depth;
    return output;
}

ostream& operator<<(ostream& strm, const Page& page)
{
    return page.dump(strm);
}
