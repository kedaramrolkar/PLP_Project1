#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include "Stack.cpp"
using namespace std;

class StackHelper
{
  public:
  Stack *stack;
  bool debugMode;
  
  StackHelper(bool dm){    
    stack=new Stack();
    debugMode=dm;  
  }
  
  void push(Node *n)  {    stack->push(n);  }

  Node* pop()  {    return stack->pop();  }
  
  void print(){    stack->print();  }
  
  void buildTree(string s, int ncount)    // BuildTree command
  {
    if(debugMode)
      cout<<"\nbuilding tree for: "<<s<<" with children: "<<ncount;
    Node *n=new Node(s);
    for(int i=0;i<ncount;i++){
      Node *p=pop();
      n->addChild(p);
      if(debugMode)
        cout<<"\nadding child: "<<p->data;
    }
    push(n);
  }
};

