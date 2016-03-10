#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include "Node.cpp"

class StackNode
{
 public:                    // StackNode will form the basic nodes for the stack
  Node *node;
  StackNode *nextNode;
  
  StackNode(Node *n){
    node=n;
    nextNode=NULL;
  }
};

class Stack
{
 public:
  StackNode *top;           // stores current top
  
  Stack(){
    top=NULL;
  }
  
  void push(Node *n)        // regular stack push operation
  {
    StackNode *s=new StackNode(n);
    if(top == NULL)
      top=s;
    else
    {
      s->nextNode=top;
      top=s;
    }
  }

  Node* pop()         // regular stack pop operation
  {
    if(top == NULL)
      return NULL;       
    
    Node *n = top->node;
    StackNode *old=top;
    top = top->nextNode;
    delete(old);
    return n;
  }
 
  void print()        // print only the data nodes present in the stack- used for Debugging purpose
  {
    StackNode *temp=top;
    while(temp!=NULL)
    {
      cout<<temp->node->data<<",";
      temp=temp->nextNode;
    }
  }      
  
  void printTree()    // print the tree for the top
  {
    StackNode *temp=top;
    if(temp!=NULL)
    {
      temp->node->printTree("");  
    }
  }      
};

