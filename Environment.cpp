#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <map>
#include <unordered_map>
using namespace std;

class Environment
{
 public:
  string name;
  Environment *parent,*child,*sibling;
  unordered_map <string, string> *hm;
  bool debugMode;

  Environment(string n, Environment *p, bool dm){
    debugMode=dm;
    name=n;
    parent=p;child=NULL;sibling=NULL;
    hm=new unordered_map<string,string>();
	  if(parent!=NULL)
	    p->addChild(this);
  }

  void addChild(Environment *c){
  	if(child==NULL){
  		child=c;
  	}
  	else{
  		c->sibling=child;
  		child=c;
  	}
  }

  void addItem(string key, string svalue){
  	if(debugMode)
      cout<<"adding "+svalue+" as "+key+" in "+name<<endl;
  	hm->insert(pair<string, string>(key, svalue));
  }

  bool isTokenKnownVariable(string token){
  	if(name=="< e0 >")
  		return false;

  	unordered_map<string, string>::const_iterator it = hm->find(token);
	if(it != hm->end())
		return true;
	return parent->isTokenKnownVariable(token);
  }

  string getVariableValue(string token){
  	if(name=="< e0 >")
  	{
  		cout<<"Error while searching for x: "+token;
  		exit(1);
  	}
  	if(debugMode)
      cout<<"searchin for "+token+" in "+name+" with size "<<hm->size()<<endl;
  	unordered_map<string, string>::const_iterator it = hm->find(token);
	if(it != hm->end()){
		if(debugMode)
      cout<<"got "+token+" "<<it->second<<endl;
		return it->second;
	}
	return parent->getVariableValue(token);
  }
};