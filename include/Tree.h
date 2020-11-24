#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include "Session.h"
using namespace std;

class Tree{
public:
    Tree(int rootLabel);
    static Tree* createTree(const Session& session, int rootLabel);
    virtual int traceTree()=0;
    void addChild(const Tree& child);
    void addChild(Tree* child);

    virtual ~Tree();
    void clear();
    Tree(const Tree &other);
    virtual Tree *clone() const=0;
    Tree(Tree &&other);
    void steal(Tree &other);
    Tree& operator=(const Tree &t);
    Tree& operator=(Tree &&t);

    void updateDepthsBFS();
    int getSizeBFS();

    int getNode() const;
    int getRank();
    int getDepth();
    vector<Tree*> *getChildren();

protected:
    int node;
    int rank;
    int depth;
    std::vector<Tree*> children;
};


class CycleTree: public Tree{
public:
    CycleTree(int rootLabel, int currCycle);
    virtual int traceTree();
    virtual Tree *clone() const;
    virtual ~CycleTree() = default;
    int getCurrCycle() const;

protected:
    int currCycle;
};

class MaxRankTree: public Tree{
public:
    MaxRankTree(int rootLabel);   
    virtual int traceTree();
    virtual Tree *clone() const;
    virtual ~MaxRankTree() = default;
};

class RootTree: public Tree{
public:
    RootTree(int rootLabel); 
    virtual int traceTree();
    virtual Tree *clone() const;
    virtual ~RootTree() = default;
};

#endif
