#include "page.h"

Page::Page()
    :depth(0)
{
}

int Page::getDepth()
{
    return depth;
}

void Page::setDepth(int depth)
{
    this->depth = depth;
}
