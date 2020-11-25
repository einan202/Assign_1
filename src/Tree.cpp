
#include "../include/Tree.h"
#include <iostream>
using namespace std;

//==========================Tree=========================

Tree::Tree(int rootLabel): node(rootLabel), rank(0), depth(0), children() {}

Tree::~Tree()

{
    clear();
}

void Tree::clear()
{
   unsigned int size = children.size();
   for(unsigned int i = 0; i < size; i++){
       if(children.at(i) != nullptr) {
            delete (children.at(i)); // due to treeSPLSiteTest
            children.at(i) = nullptr;
       }
   }
}

Tree& Tree::operator=(const Tree &other)
{
    if(this != &other){
        clear();
        node = other.node;
        rank = other.rank;
        depth = other.depth;
        unsigned int count = other.children.size();
        for (unsigned int i = 0; i < count; ++i) {
            children.push_back(other.children.at(i)->clone());
        }
    }
    return *this;
}

Tree &Tree::operator=(Tree &&other) {
    if(this != &other){
        clear();
        node = other.node;
        rank = other.rank;
        depth = other.depth;
        unsigned int count = other.children.size();
        for (unsigned int i = 0; i < count; ++i) {
            children.push_back(other.children.at(i)->clone());
        }
        steal(other);
    }
    return *this;
}

void Tree::steal(Tree &other)
{
    unsigned int count=other.children.size();
    for(unsigned int i = 0; i<count; i++)
        children.at(i) = nullptr;
}

Tree * Tree::createTree(const Session &session, int rootLabel)
{
    Tree *p;
    TreeType tType = session.getTreeType();
    if(tType==MaxRank)
        p = new MaxRankTree(rootLabel);
    if(tType==Cycle)
        p = new CycleTree(rootLabel,0);
    if(tType==Root)
        p = new RootTree(rootLabel);
    return p;
}

void Tree::addChild(const Tree &child)
{
    Tree* copy = child.clone();
    addChild(copy);
}

void Tree::addChild(Tree *child)
{
    children.push_back(child);
    child->updateDepthsBFS();
    rank++;
}

int Tree::getNode() const
{
    return node;
}

Tree::Tree(Tree &&other):
  node(other.node), rank(other.rank), depth(other.depth), children(move(other.children))
{
    steal(other);
}

vector<Tree *> *Tree::getChildren()
{
    return &children;
}

int &Tree::getRank()
{
    return rank;
}

int &Tree::getDepth()
{
    return depth;
}

void Tree::updateDepthsBFS()
{
    queue<Tree*> q;
    q.push(this);
    while(!q.empty()){
        Tree* curr = q.front(); q.pop();
        unsigned int count = curr->getChildren()->size();
        for (unsigned int i = 0; i < count; ++i){
            Tree* child = curr->getChildren()->at(i);
            q.push(child);
        }
        curr->getDepth()++;
    }
}

//int Tree::getSizeBFS()
//{
//    int size = 0;
//    queue<Tree*> q;
//    q.push(this);
//    while(!q.empty()){
//        Tree* curr = q.front(); q.pop();
//        unsigned int count=curr->getChildren()->size();
//        for (unsigned int i = 0; i < count; ++i)
//            q.push(curr->getChildren()->at(i));
//        size= size + 1;
//    }
//    return size;
//}

Tree::Tree(const Tree &other):
     node(other.node), rank(other.rank), depth(other.depth), children()
{
    unsigned int count = other.children.size();
    for(unsigned int i=0; i < count; i++){
        children.push_back(other.children.at(i)->clone());
    }
}

//==========================RootTree=====================================

RootTree::RootTree(int rootLabel):Tree(rootLabel){}

Tree * RootTree::clone() const
{
    return (new RootTree(*this));
}

int RootTree::traceTree()
{
    return node;
}

//==========================CycleTree=====================================

CycleTree::CycleTree(int rootLabel, int currCycle):Tree(rootLabel),currCycle(currCycle){}

Tree * CycleTree::clone() const
{
    return (new CycleTree(*this));
}

int CycleTree::traceTree() {// if 0 - root, else go-left currCycle times
    int output = node;
    Tree *curr = this;
    int length = curr->getChildren()->size();
    unsigned int cycles = getCurrCycle();
    for (unsigned int i = 0; (i < cycles) & (length != 0); ++i) {
        output = curr->getChildren()->at(i)->getNode();
        curr = curr->getChildren()->at(i);
        length = curr->getChildren()->size();
    }
    return output;
}

    int CycleTree::getCurrCycle() const {
        return currCycle;
    }

//==========================MaxRankTree=====================================

MaxRankTree::MaxRankTree(int rootLabel):Tree(rootLabel){}

Tree *MaxRankTree::clone() const
{
        return (new MaxRankTree(*this));
}

void MaxRankTree::updateSameRankBFS(vector<Tree*> *sameRank)
{
    int maxRank = sameRank->at(0)->getRank();
    queue<Tree*> q;
    q.push(this);
    while(!q.empty()){
        Tree* curr = q.front(); q.pop();
        if(curr->getRank() >= maxRank)
        {
          if(curr->getRank() > maxRank) {
              sameRank->clear();
              maxRank = curr->getRank();
          }
          if(curr->getNode() != getNode())
              sameRank->push_back(curr);
        }
        unsigned int count=curr->getChildren()->size();
        for (unsigned int i = 0; i < count; ++i)
            q.push(curr->getChildren()->at(i));
    }
}

int MaxRankTree::traceTree()
{
    vector<Tree*> sameRank;
    sameRank.push_back(this);
    updateSameRankBFS(&sameRank);
    if(sameRank.size() == 1)
        return sameRank.at(0)->getNode();
    else
    {
        Tree* min = sameRank.at(0);
        for (auto &k : sameRank) {
            if (min->getDepth() > k->getDepth())
                min = k;
            else if (min->getDepth() == k->getDepth()) {
                if (min->getNode() > k->getNode())
                    min = k;
            }
        }
        return min->getNode();
    }
}