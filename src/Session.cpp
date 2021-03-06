
#include "../include/Session.h"
#include <fstream>
#include <iostream>
#include "../include/Agent.h"
using namespace std;
using namespace nlohmann;

//========================Session===============================

Session::Session(const std::string& path):
   done(),index(0), g(vector<vector<int>>()),treeType(), agents(),
     simulateCycle(0), terminated(false),infectedNode()
{
    std::ifstream i(path);
    json j;
    i >> j;

    g = Graph(j["graph"]);

    if (j["tree"].front() == "M")
        treeType = MaxRank;
    if (j["tree"].front() == "C")
        treeType = Cycle;
    if (j["tree"].front() == "R")
        treeType = Root;

    infectedNode = queue<int>();

    for(pair<string, int> pair : j["agents"]) {
        if (pair.second == -1)
            agents.push_back(new ContactTracer());
        else
            agents.push_back(new Virus(pair.second));
    }
    done = vector<bool>(agents.size(), false);
}

void Session::simulate() {

    while (!terminated) {

        int count = agents.size();
        for (int i = 0; i < count; ++i) {
            index = i;
            (agents.at(i))->act(*this);
        }

        bool check = true;
        count = done.size();
        for (int k = 0; (k < count) & (check); ++k) {
            if (!done.at(k))
                check = false;
        }

        if (check) {
            int size = g.vecs.size();
            for (int a = 0; (a < size) & (check); ++a) {
                if(g.vecs.at(a) == 1)
                    check = false;
            }
        }
        terminated = check;
        simulateCycle++;
    }
    makeOutput();
}

void Session::addAgent(const Agent &agent)
{
    Agent *toAdd=agent.clone();
    agents.push_back(toAdd);
}

void Session::enqueueInfected(int node)
{
    infectedNode.push(node);
}

int Session::dequeueInfected()
{
    int output = infectedNode.front();
    infectedNode.pop();
    return output;
}

void Session::setGraph(const Graph &graph)
{
    g = Graph(graph);
}

Graph *Session::getGraph()
{
    return &g;
}

TreeType Session::getTreeType() const
{
    return treeType;
}

vector<Agent *> Session::getAgents()
{
    return agents;
}

int& Session::getSimulateCycle()
{
    return simulateCycle;
}

std::vector<bool> * Session::getDone()
{
    return &done;
}

queue<int> * Session::getInfected()
{
    return &infectedNode;
}

Session::~Session()
{
    clear();
}

void Session::clear()
{
    unsigned int size = agents.size();
    for (unsigned int i = 0; i < size; ++i) {
        if (agents.at(i) != nullptr) {
            delete (agents.at(i));
            agents.at(i) = nullptr;
        }
    }
}

Session & Session::operator=(const Session &other)
{
    if(this!=&other) {
        clear();
        treeType = other.treeType;
        g = Graph(other.g);
        terminated = other.terminated;
        done = other.done;
        infectedNode = other.infectedNode;
        index = other.index;
    }
    unsigned int size=other.agents.size();
    for (unsigned int i = 0; i <size ; ++i) {
        Agent *newAgent = other.agents[i]->clone();
        agents.push_back(newAgent);
    }
    return *this;
}

Session & Session::operator=(Session &&other)
{
    if(this!= &other){
        clear();
        treeType=other.treeType;
        g=other.g;
        terminated=other.terminated;
        done=other.done;
        infectedNode=other.infectedNode;
        index=other.index;
        unsigned int count=agents.size();
        for (unsigned int i = 0; i <count ; ++i) {
            Agent *newAgent = other.agents[i]->clone();
            agents.push_back(newAgent);
        }
        steal(other);
    }
    return *this;
}

void Session::steal(Session &other)
{
    unsigned int size = agents.size();
    for (unsigned int i = 0; i < size; ++i) {
        agents.at(i) = nullptr;
    }
}

void Session::makeOutput()
{
    json k;
    vector<int> outputInfected = this->outputInfected();
    vector<vector<int>> outputGraph = this->outputGraph();
    k["graph"] = outputGraph;
    k["infected"] = outputInfected;
    cout << k["infected"] << endl;
    for(auto& a : k["graph"])
        cout << a << endl;
    ofstream  u("output.json");
    u << k;
}

std::vector<int> Session::outputInfected()
{
    vector<int>output;
    unsigned int count = g.vecs.size();
    for (unsigned int i = 0; i < count ; ++i) {
        if(g.vecs.at(i) == 2)
            output.push_back(i);
    }
    return output;
}

std::vector<std::vector<int>> Session::outputGraph()
{
    unsigned int n = g.getEdges().size();
    vector<vector<int>> output(n,vector<int>(n,0));
    for(unsigned int i = 0 ; i < n ; ++i){
        unsigned int in = g.getEdges().at(i).size();
        for(unsigned int j = 0 ; j < in ; ++j) {
            int value = g.getEdges().at(i).at(j);
            output.at(i).at(value) = 1;
            output.at(value).at(i) = 1;
        }
    }
    return output;
}

int Session::toInfect(int father)
{
    int toInfect = -1;
    unsigned int fatherEdgesSize = g.getEdges().at(father).size();
    for (unsigned int i = 0; (i < fatherEdgesSize) & (toInfect == -1); ++i) {
        int neighbor = g.getEdges().at(father).at(i);
        bool notVirus = true;
        int status = getGraph()->vecs.at(neighbor);
        if (status == 0){

            for(auto& agent: agents)
                if(agent->getVertex() == neighbor)
                    notVirus = false;

            if(notVirus)
               toInfect = neighbor;
        }
    }
    return toInfect;
}

Session::Session(const Session &other):
  done(other.done), index(other.index), g(vector<vector<int>>()), treeType(other.treeType),
   agents(), simulateCycle(other.simulateCycle),
    terminated(other.terminated), infectedNode(other.infectedNode)
{
    g = Graph(other.g);
    unsigned count= other.agents.size();
    for (unsigned int i = 0; i < count ; ++i) {
        Agent *newAgent = other.agents[i]->clone();
        agents.push_back(newAgent);
    }
}

Session::Session(Session &&other):
  done(other.done), index(other.index), g(vector<vector<int>>()), treeType(other.treeType),
   agents(move(other.agents)), simulateCycle(other.simulateCycle),
    terminated(other.terminated),infectedNode(other.infectedNode)
{
    g = other.g;
    steal(other);
}




