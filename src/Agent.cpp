
#include "../include/Agent.h"
#include "../include/Tree.h" // Added
#include <iostream>
using namespace std;

//=================Agent===================

Agent::Agent() {}
Agent::~Agent() {}

//=================Virus===================

Virus::Virus(int nodeInd) : Agent(), nodeInd(nodeInd){}

void Virus::act(Session &session)
{
    if (!session.getDone()->at(session.index)) {
        bool act = false;
        Graph *graph = session.getGraph();
        int node = this->nodeInd;
        int status = graph->vecs.at(node);
        if (status == 0) {
            graph->vecs.at(node) = 1;
            int toInfect = session.toInfect(node);
            if (toInfect != -1)
            {
                const Agent *a = new Virus(toInfect);
                graph->vecs.at(toInfect) = 1;
                session.addAgent(*a);
                session.getDone()->push_back(false);
                session.getDone()->at(session.index) = false;
                act = true;
                delete (a);
            }
            status++;
        }

        if (status == 1) {
            graph->vecs.at(node) = 2;
            session.enqueueInfected(node);
            if(!act) {
                int toInfect = session.toInfect(node);
                if (toInfect != -1) {
                    const Agent *a = new Virus(toInfect);
                    graph->infectNode(toInfect);
                    session.addAgent(*a);
                    session.getDone()->push_back(false);
                    session.getDone()->at(session.index) = false;
                    act = true;
                    delete (a);
                }
            }
            status++;
        }
        if (status == 2)  {
           if(!act) {
               int toInfect = session.toInfect(node);
               if (toInfect == -1)
                   session.getDone()->at(session.index) = true;
               else
               {
                   const Agent *a = new Virus(toInfect);
                   graph->infectNode(toInfect);
                   session.addAgent(*a);
                   session.getDone()->push_back(false);
                   session.getDone()->at(session.index) = false;
                   delete (a);
               }
           }
        }
    }
}

int Virus::getNodeInd() const
{
    return nodeInd;
}

Agent *Virus::clone() const
{
    return (new Virus(*this));
}

//=================Contact Tracer===================

ContactTracer::ContactTracer() : Agent() {}

void ContactTracer::act(Session &session)
{
    if(!session.getInfected()->empty()){
        int node=session.dequeueInfected();
        Tree *tree=session.getGraph()->BFS(session,node);
        if (tree->getRank() != 0) {
            if(session.getTreeType() == Cycle)
                tree->updateCyclesBFS(session.getSimulateCycle());
            int disconnected = tree->traceTree();
            session.getGraph()->removeNodeEdges(disconnected);
        }
        delete (tree);
    }
    session.getDone()->at(session.index) = true;
}

Agent *ContactTracer::clone() const
{
    return (new ContactTracer());
}



