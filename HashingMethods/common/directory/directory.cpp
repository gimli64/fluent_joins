#include "directory.h"

Directory::Directory()
    :globalDepth(0)
{
}

string* Directory::getValue(int key, string value) {
    return getBucket(key).getValue(value);
}
