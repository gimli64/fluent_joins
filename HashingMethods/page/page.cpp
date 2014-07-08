#include "page.h"

Page::Page()
    :depth(0)
{
}

vector<string> Page::getValue(string key)
{
    return vector<string>();
}

void Page::putCouple(Couple couple) {}

int Page::getDepth()
{
    return depth;
}

void Page::setDepth(int depth)
{
    this->depth = depth;
}
