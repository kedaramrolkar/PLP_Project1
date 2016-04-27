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

  Node* clone(){
    Node *n=new Node(data);
    if(sibling!=NULL)
      n->addSibling(sibling->clone());
    if(child!=NULL)
      n->addChild(child->clone());
    return n;
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

  void addSibling(Node *n){       // Add the given node as a child to current
    if(sibling==NULL)
      sibling=n;
    else{
      Node *p=sibling;
      n->sibling=p;
      sibling=n;
    }
  }
  
  void printTree(string prestring){   // Print the whole tree for the node in the required format
    cout<<prestring<<data<<"\n";
    if(child!=NULL)
      child->printTree(prestring+".");
    if(sibling!=NULL)
      sibling->printTree(prestring);
  }

  int childCount(){
    if(child==NULL)
      return 0;
    int count=0;
    Node* n=child;
    while(n!=NULL){
      count++;
      n=n->sibling;
    }
    return count;
  }

  Node* standardizeNode(){   
    if(sibling!=NULL)
      sibling = sibling->standardizeNode();

    if(child!=NULL){
      child=child->standardizeNode();
    }

    if(data=="let" || data=="where" || data=="within" || data=="function_form" 
        || data=="@" || data=="and" || (data=="lambda" && childCount() >2) || data=="rec")
    {
      Node *n=NULL;
      if(data=="let")
        n = standardize_let();
      else if(data=="where")
        n = standardize_where();
      else if(data=="within")
        n = standardize_within();
      else if(data=="function_form")
        n = standardize_function_form();
      else if(data=="@")
        n = standardize_atTheRate();
      else if(data=="and")
        n = standardize_and();
      else if(data=="lambda" && childCount() >2)
        n = child->standardize_lambda();
      else if(data=="rec")
        n = standardize_rec();
      n->addSibling(sibling);
      return n;
    }
    return this;
  }

  Node* standardize_let(){
    if(child==NULL || child->data!="="){
      cout<<"\nThe node '=' was not found for 'let'";
      exit(1);
    }
      
    Node *x=child->child, *e=child->child->sibling, *p=child->sibling;
    p->sibling=NULL;  x->sibling=NULL;  e->sibling=NULL;

    if(x==NULL || e==NULL || p==NULL){
      cout<<"\nThe node 'x,p,e' was not found for 'let'";
      exit(1);
    }
    
    Node *g=new Node("gamma"), *l=new Node("lambda");
    g->addChild(e);      g->addChild(l);
    l->addChild(p);      l->addChild(x);
    return g;
  }

  Node* standardize_where(){
    if(child==NULL || child->sibling==NULL || child->sibling->data!="="){
      cout<<"\nThe node '=' was not found for 'let'";
      exit(1);
    }
      
    Node *p=child, *x=child->sibling->child, *e=child->sibling->child->sibling;
    p->sibling=NULL;  x->sibling=NULL;  e->sibling=NULL;

    if(x==NULL || e==NULL || p==NULL){
      cout<<"\nThe node 'x,p,e' was not found for 'where'";
      exit(1);
    }
     
    Node *g=new Node("gamma"), *l=new Node("lambda");
    g->addChild(e);      g->addChild(l);
    l->addChild(p);      l->addChild(x);
    return g;
  }

  Node* standardize_within(){
    if(child==NULL || child->data!="=" || child->sibling==NULL || child->sibling->data!="="){
      cout<<"\nThe node '=' was not found for 'within'";
      exit(1);
    }
      
    Node *x1=child->child, *e1=child->child->sibling, *x2=child->sibling->child, *e2=child->sibling->child->sibling;
    if(x1==NULL || x2==NULL || e1==NULL || e2==NULL){
      cout<<"\nThe node 'x1,x2,e1,e2' was not found for 'within'";
      exit(1);
    }
      
    Node *n=new Node("="), *g=new Node("gamma"), *l=new Node("lambda");
    n->addChild(g);      n->addChild(x2);
    g->addChild(e1);     g->addChild(l);
    l->addChild(e2);     l->addChild(x1);
    return n;
  }

  Node* standardize_and(){
    int count=childCount();
    Node* n=child;
    Node *equals=new Node("="), *comma=new Node(","), *tau=new Node("tau");
    equals->addChild(tau); equals->addChild(comma);

    Node *prevx=NULL, *preve=NULL, *storedx=NULL, *storede=NULL;
    for(int i=0;i<count;i++){
      Node *x=n->child, *e=n->child->sibling;
      x->sibling=NULL;  e->sibling=NULL;  

      if(prevx == NULL)
      {
        prevx=x;  preve=e;  storedx=x;  storede=e;
      }
      else
      {
        prevx->addSibling(x); preve->addSibling(e);
      }
      prevx=x;  preve=e;
      n=n->sibling;
    }
    comma->addChild(storedx);
    tau->addChild(storede);
    return equals;
  }

  Node* standardize_atTheRate(){
    if(childCount() !=3){
      cout<<"\nIncorrect Child count for '@'";
      exit(1);
    }
      
    Node *e1=child, *n=child->sibling, *e2=child->sibling->sibling;
    e1->sibling=NULL;  n->sibling=NULL;  e2->sibling=NULL;

    Node *g1=new Node("gamma"), *g2=new Node("gamma");
    g2->addChild(e1);      g2->addChild(n);
    g1->addChild(e2);      g1->addChild(g2);
    return g1;
  }

  Node* standardize_function_form(){
    if(childCount() <3){
      cout<<"\nNot enough Child Nodes for 'function_form'";
      exit(1);
    }
      
    Node *p=child, *s=child->sibling;
    p->sibling=NULL;

    Node *n=new Node("=");
    n->addChild(s->standardize_lambda());      n->addChild(p);
    return n;
  }

  Node* standardize_lambda(){
    Node *l=new Node("lambda");
    if(sibling->sibling!=NULL)
      l->addChild(sibling->standardize_lambda());
    else
      l->addChild(sibling);
    l->addChild(this);
    return l;
  }

  Node* standardize_rec(){
    if(childCount() !=1 || child->data!="=" )
    {
      cout<<"\nThe node 'rec' is incorrect";
      exit(1);
    }

    Node *equals=new Node("="), *g=new Node("gamma"), *l=new Node("lambda"), *ystar=new Node("Ystar");
    Node *x=child->child, *e=child->child->sibling;
    x->sibling=NULL;  e->sibling=NULL;
    
    equals->addChild(g);
    g->addChild(l);  
    g->addChild(ystar);
    

    l->addChild(e);  
    l->addChild(x); 
    Node *x2=x->clone(); 
    equals->addChild(x2); 
    return equals;
  }
};

