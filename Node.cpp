#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

class Node
{
 public:
  string data;            //Token for Node
  Node *child,*sibling;
  
  Node(string s){
    data=s;
    child=NULL;
    sibling=NULL;
  }
  
  void addChild(Node *n){       // Add the given node as a child to current
    if(child==NULL)
      child=n;
    else{
      Node *p=child;
      n->sibling=p;
      child=n;
    }
  }
  
  void printTree(string prestring){   // Print the whole tree for the node in the required format
    cout<<prestring<<data<<"\n";
    if(child!=NULL)
      child->printTree(prestring+".");
    if(sibling!=NULL)
      sibling->printTree(prestring);
  }
};

