#ifndef RANK_TREE
#define RANK_TREE

#include "SearchTree.h"
#include "Lecture.h"

class RankTree : public SearchTree<Lecture>
{
public:
    bool insert(int key, const Lecture& lect) override;
    bool remove(int key) override;
    Node findByOrder(int i);
};

#endif