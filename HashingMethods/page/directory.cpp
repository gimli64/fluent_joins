#include "directory.h"

Directory::Directory(HashTable *hasher)
    :level(0), parent(0)
{
    factory = BucketFactory<Bucket>::getInstance();
    if (hasher != 0) {
        this->hasher = hasher;
        pages.push_back(factory->newBucket());
    }
}

vector<string> Directory::getValue(size_t hash, string key)
{
    try {
        return ((Bucket*) getBucket(hash))->getValue(hash, key);
    } catch (string &e) {
        throw e;
    }
}

void Directory::putCouple(size_t hash, Couple couple)
{
    Page *page = getPage(hash);
    if(!page->isBucket()) {
        page->putCouple(hash, couple);
        return;
    }

    if (!page->isFull()) {
        page->putCouple(hash, couple);
        return;
    }
    if (page->getDepth() < depth) {
        split((Bucket*) page);
        putCouple(hash, couple);
        return;
    }
    if (depth < DEPTH_LIMIT) {
        if (parent == 0) {// Working with root directory
            doubleSize();
            putCouple(hash, couple);
        }
        else {
            parent->splitInnerNode(hash);
            putCouple(hash, couple);
        }
        return;
    }
//    cout << *this << endl;
//    cout << "insert new level" << endl;
    insertNewLevel(hash);
//    cout << *this << endl;
    putCouple(hash, couple);
}

void Directory::doubleSize()
{
    size_t oldSize = pages.size();
    pages.reserve(2 * oldSize);
    for (size_t i = 0; i < oldSize; i++) {
        pages.push_back(pages.at(i));
    }
    depth++;
}

void Directory::splitInnerNode(size_t hash)
{
    size_t index = pageIndex(hash);
    Page *child = pages[index];
    int childDepth = child->getDepth();
    int indexFirstBits = index & ((1 << childDepth) - 1);
    for (int i = 0; i < pages.size(); i++) {
        if ((i & ((1 << childDepth) - 1)) == indexFirstBits)
            pages[i]->doubleSize();
    }
}

void Directory::insertNewLevel(size_t hash)
{
    int firstBit = pageIndex(hash) & 1;
    for(int i = 0; i < pages.size(); i++) {
        if ((i & 1) == firstBit) {
            if (pages[i]->getDepth() < depth) {
                cout << "LULULULULU" << endl;
            }
            if (pages[i]->getDepth() == depth - 1) {
                cout << "LALALALALA" << endl;
                split((Bucket*) pages[i]);
            }
        }
    }
    for(int i = 0; i < pages.size(); i++) {
        if ((i & 1) == firstBit) {
            Directory *newDir = new Directory();
            newDir->hasher = hasher;
            newDir->setDepth(1);
            newDir->setLevel(getLevel() + 1);
            newDir->parent = this;

            Page *previousBucket = pages[i];
            newDir->pages.push_back(previousBucket);
            newDir->pages.push_back(previousBucket);
            int bucketDepth = previousBucket->getDepth();
            previousBucket->setDepth(0);
//            if (bucketDepth == depth - 1) {
//                cout << "LALALALALA" << endl;
//                split((Bucket*) previousBucket);
//            }
//            if (bucketDepth < depth) {
//                cout << "LULULULU" << endl;
//                for (int j = 0; j < pages.size(); j++) {
//                    if (pages[j] == previousBucket)
//                        pages[j] = newDir;
//                }
//            } else {
//                pages[i] = newDir;
//            }
            pages[i] = newDir;
        }
    }
}

void Directory::split(Bucket* bucket)
{
    Bucket *newBucket1 = factory->newBucket();
    Bucket *newBucket2 = factory->newBucket();
    vector<Couple> values = bucket->getAllValues();

    for (vector<Couple>::iterator it = values.begin(); it != values.end(); ++it) {
        size_t h = pageIndex(hasher->getMultikeyHash(*it));
        if ((h | (1 << bucket->getDepth())) == h)
            newBucket2->putCouple(h, *it);
        else
            newBucket1->putCouple(h, *it);
    }

    vector<int> l;
    for(vector<Bucket>::size_type i = 0; i < pages.size(); i++) {
        if(pages.at(i) == bucket)
            l.push_back(i);
    }

    for(vector<int>::iterator it = l.begin(); it != l.end(); ++it) {
        if ((*it | (1 << bucket->getDepth())) == *it) {
            pages.at(*it) = newBucket2;
        }
        else {
            pages.at(*it) = newBucket1;
        }
    }

    newBucket1->setDepth(bucket->getDepth() + 1);
    newBucket2->setDepth(newBucket1->getDepth());
    factory->deleteBucket(bucket);
}

size_t Directory::pageIndex(size_t hash)
{
    return (hash >> (DEPTH_LIMIT * getLevel())) & ((1 << depth) - 1);
}

Page *Directory::getPage(size_t hash)
{
    return pages.at(pageIndex(hash));
}

Page *Directory::getBucket(size_t hash)
{
    return pages.at(pageIndex(hash))->getBucket(hash);
}

int Directory::getLevel()
{
    return level;
}

void Directory::setLevel(int level)
{
    this->level = level;
}

vector<Bucket *> Directory::getBuckets()
{
    //    set<Bucket *> uniqueBuckets = set<Bucket *>(pages.begin(), pages.end());
    //    vector<Bucket *> buckets;
    //    buckets.reserve(uniqueBuckets.size());
    //    for (set<Bucket*>::iterator it = uniqueBuckets.begin(); it != uniqueBuckets.end(); ++it) {
    //        buckets.push_back(*it);
    //    }

    //    return buckets;
}

int Directory::getGlobalDepth()
{
    int maxPageDepth = 0;
    for(int i = 0; i < pages.size(); i++) {
        if(!pages[i]->isBucket()) {
            int pageDepth = pages[i]->getGlobalDepth();
            if (pageDepth > maxPageDepth)
                maxPageDepth = pageDepth;
        }
    }

    return depth + maxPageDepth;
}

string Directory::className() const
{
    return "Directory ";
}

ostream& Directory::dump(ostream& strm) const
{
    ostream& output = Page::dump(strm) << ", level " << lexical_cast<string>(level) << endl;
    for(int i = 0; i < pages.size(); i++) {
        for(int j = 0; j < level + 1; j++)
            output << "--";
        output << "> " << *(pages[i]);
    }
    return output;
}
