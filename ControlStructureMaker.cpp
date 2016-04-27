#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <queue>
using namespace std;

class ControlStructureMaker
{
 public:
  vector<string> ls;
  queue< Node* > lambdas;
  int index;

  ControlStructureMaker(Node *t)
  {
  	index=0;
  	lambdas.push(t);
  	while(lambdas.size() > 0){
  		Node* startNode=lambdas.front();
  		lambdas.pop();
  		ls.push_back(createControlStructures(startNode));
  	}
  }

  void print(){
  	cout<<endl;
  	int i=0;
  	for(vector<string>::iterator it = ls.begin(); it != ls.end(); ++it) {   	
		cout <<"delta "<< i++<<" = "<<*it<<endl;
    }
    cout<<endl;
  }

  string createControlStructures(Node* t)
  {
  	string s="";
  	if(t->data=="lambda"){
  		string ss=to_string(++index);
  		lambdas.push(t->child->sibling);
  		t->child->sibling=NULL;

  		if(t->child->data!=",")
  			s="< lambda "+ss+" " + createControlStructures(t->child);
  		else
  			s= "< lambdaComma "+ to_string(t->child->childCount()) +" "+ss+" " + createControlStructures(t->child->child);

  		s+= " > ";
  		if(t->sibling!=NULL)
      		s+=" "+createControlStructures(t->sibling);
  		return s;
  	}
  	else if(t->data[0]=='<'){
  		string curr=t->data;
  		int ind=1;
  		if(curr.substr(1,2)=="ID")
  			ind=4;
  		else if(curr.substr(1,2)=="IN")
  			ind=5;
  		else if(curr.substr(1,2)=="ST")
  			ind=5;
  		s=curr.substr(ind,curr.length());
  		s.erase(s.end()-1, s.end());
  	}
  	else if(t->data=="tau"){
  		int n=t->childCount();
  		s="< tau " + to_string(n) + " > "; 
  		s+=createControlStructures(t->child);

  		if(t->sibling!=NULL)
      		s+=" "+createControlStructures(t->sibling);
  		return s;
  	}
  	else if(t->data=="->"){
  		Node* beta=t->child;

  		lambdas.push(t->child->sibling);
  		//int firstIndex = lambdas.size()+1;
  		int firstIndex = ++index;
  		lambdas.push(t->child->sibling->sibling);
  		//int secondIndex = lambdas.size()+1;
  		int secondIndex = ++index;

  		t->child->sibling->sibling=NULL;	t->child->sibling=NULL;
  		s="< delta " + to_string(firstIndex) + " > < delta " + to_string(secondIndex) + " > beta "; 
  		s+=createControlStructures(beta);
  		//cout<<"pushed "+s+" "<<index<<endl;
  		if(t->sibling!=NULL)
      		s+=" "+createControlStructures(t->sibling);
  		return s;
  	}
  	else
  		s=t->data;

    if(t->child!=NULL){
      s= s+" "+ createControlStructures(t->child);
  	}
    if(t->sibling!=NULL)
      s= s+" "+ createControlStructures(t->sibling);
  	return s;
  }
};