
#include "../include/Session.h"
#include "../include/Agent.h" // Added
#include "../include/Tree.h" // Added
#include <iostream>
using namespace std;
using namespace nlohmann;

int main(int argc, char** argv)
{
    if (argc != 2) {
        cout << "usage cTrace <config_path>" << endl;
        return 0;
    }

    Session sess(argv[1]);
    sess.simulate();
    return 0;
}
//================Unused_Functions=======================

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

//bool Graph::infectNextNode(int father)
//{
//   bool output = false;
//   unsigned int fatherEdgesSize = edges.at(father).size();
//   for (unsigned int i = 0; (i < fatherEdgesSize) & !output; ++i) {
//        int neighbor = edges.at(father).at(i);
//        int status = vecs.at(neighbor);
//        if (status == 0) {
//            infectNode(neighbor);
//            output = true;
//        }
//   }
//   return output;
//}