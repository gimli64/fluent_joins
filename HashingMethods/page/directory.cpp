#include "directory.h"

Directory::Directory(HashTable *hasher)
    :level(0), parent(0), hashPrefix(0)
{
    factory = BucketFactory<Bucket>::getInstance();
    if (hasher != 0) {
        this->hasher = hasher;
        pages.push_back(factory->newBucket());
    }
}

vector<string> Directory::getValue(size_t hash)
{
    try {
        return getBucket(hash)->getValue(hash);
    } catch (string &e) {
        throw e;
    }
}

void Directory::putCouple(size_t hash, Couple couple)
{
    if (hasher->getNumberBHFs() < (DEPTH_LIMIT * level + depth)) {
        hasher->addBHF();
        hash = hasher->getMultikeyHash(couple);
    }

    Page *page = getPage(hash);
    if(!page->isBucket()) {
        page->putCouple(hash, couple);
        return;
    }
    Bucket *bucket = (Bucket*) page;
    if (!bucket->isFull()) {
        bucket->putCouple(hash, couple);
        return;
    }
    if (bucket->getDepth() < depth) {
        split(bucket);
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
    insertNewLevel(hash);
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
    size_t childIndex = pageIndex(hash);
    Page *child = pages[childIndex];
    int childDepth = child->getDepth();
    int indexFirstBits = childIndex & ((1 << childDepth) - 1);
    for (int i = 0; i < pages.size(); i++) {
        if ((i & ((1 << childDepth) - 1)) == indexFirstBits)
            ((Directory*) pages[i])->doubleSize();
    }
}

void Directory::insertNewLevel(size_t hash)
{
    size_t childIndexFirstBit = pageIndex(hash) & 1;
    splitAllBuckets(childIndexFirstBit);
    for(int i = 0; i < pages.size(); i++) {
        if ((i & 1) == childIndexFirstBit) {
            Directory *newDir = new Directory();
            newDir->hasher = hasher;
            newDir->setDepth(1);
            newDir->level = this->level + 1;
            newDir->parent = this;
            newDir->hashPrefix = i;
            pages[i]->setDepth(0);
            newDir->pages.push_back(pages[i]);
            newDir->pages.push_back(pages[i]);
            pages[i] = newDir;
        }
    }
}

void Directory::splitAllBuckets(size_t childIndexFirstBit)
{
    bool allBucketsSplit = false;
    while(!allBucketsSplit) {
        allBucketsSplit = true;
        for(int i = 0; i < pages.size(); i++) {
            if ((i & 1) == childIndexFirstBit && pages[i]->getDepth() < DEPTH_LIMIT) {
                if (pages[i]->getDepth() < DEPTH_LIMIT - 1)
                    allBucketsSplit = false;
                split((Bucket*) pages[i]);
            }
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
        if ((*it | (1 << bucket->getDepth())) == *it)
            pages.at(*it) = newBucket2;
        else
            pages.at(*it) = newBucket1;
    }

    newBucket1->setDepth(bucket->getDepth() + 1);
    newBucket2->setDepth(newBucket1->getDepth());
    factory->deleteBucket(bucket);
}

size_t Directory::pageIndex(size_t hash)
{
    return (hash >> (DEPTH_LIMIT * level)) & ((1 << depth) - 1);
}

Page *Directory::getPage(size_t hash)
{
    return pages.at(pageIndex(hash));
}

Page *Directory::getBucket(size_t hash)
{
    return pages.at(pageIndex(hash))->getBucket(hash);
}

vector<Bucket *> Directory::getBuckets()
{
    vector<Bucket *> buckets;
    for (int i = 0; i < pages.size(); i++) {
        if (pages[i]->isBucket())
            buckets.push_back((Bucket *) pages[i]);
        else {
            vector<Bucket*> pageBuckets = ((Directory*) pages[i])->getBuckets();
            buckets.insert(buckets.end(), pageBuckets.begin(), pageBuckets.end());
        }
    }
    return buckets;
}

int Directory::getGlobalDepth()
{
    int maxPageDepth = 0;
    for(int i = 0; i < pages.size(); i++) {
        if(!pages[i]->isBucket()) {
            int pageDepth = ((Directory*) pages[i])->getGlobalDepth();
            if (pageDepth > maxPageDepth)
                maxPageDepth = pageDepth;
        }
    }
    return depth + maxPageDepth;
}

double Directory::pageSize() const
{
    double size = pow(2.0, depth) / pow(2.0, DEPTH_LIMIT);
    for (int i = 0; i < pages.size(); i++) {
        if (!pages[i]->isBucket())
            size += ((Directory*)pages[i])->pageSize();
    }
    return size;
}

string Directory::className() const
{
    return "Directory ";
}

ostream& Directory::dump(ostream& strm) const
{
    ostream& output = Page::dump(strm) << ", level " << lexical_cast<string>(level);
    if (parent != 0)
        output << ", hash prefix " << bitset<13>(hashPrefix);
    output << ", page size " << pageSize() << endl;
    for(int i = 0; i < pages.size(); i++) {
//        if (!pages[i]->isBucket()) {
            for(int j = 0; j < level + 1; j++)
                output << "--";
            output << "> " << *(pages[i]);
//        }
    }
    return output;
}
