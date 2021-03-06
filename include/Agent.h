#ifndef AGENT_H_
#define AGENT_H_

#include <vector>
#include <queue>
#include "Session.h"
using namespace std;

class Agent{
public:
    Agent();
    virtual void act(Session& session)=0;
    virtual Agent* clone() const = 0;
    virtual ~Agent();
    virtual int& getVertex() = 0;
};

class ContactTracer: public Agent{
public:
    ContactTracer();
    virtual void act(Session& session);
    virtual Agent* clone() const;
    virtual ~ContactTracer() = default;
    virtual int& getVertex();

private:
    int vertex;
};

class Virus: public Agent{
public:
    Virus (int nodeInd);
    virtual void act(Session& session);
    virtual Agent* clone() const;
    virtual ~Virus() = default;
    int getNodeInd() const;
    virtual int& getVertex();

private:
    const int nodeInd;
    int vertex;
};

#endif
