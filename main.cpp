#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include "parser.cpp"
#include "ControlStructureMaker.cpp"
#include "CSEMachine.cpp"
using namespace std;

void listInput(string fileLocation);
void treePrint(string fileLocation, bool debugMode, bool treePrint);

int main(int argc, char** argv) {  

  if (argc <= 1 || argc>4)  exit(1); //only allow 3 or 4 parameters (fn,switch+,fileLocation)

  if(argc==2){
    string fileLocation=argv[1];
    treePrint(fileLocation,false,false);
    return 0;
  }

  string switch1=argv[1];	
  if(argc==3){
    string fileLocation=argv[2];
    if(switch1=="-l")  // if 3 parameters with -l switch -> Print input and return
    {
      listInput(fileLocation);  
    }
    else if(switch1=="-ast")   // if 3 parameters with -ast switch -> Print Tree and return
    {
      treePrint(fileLocation,false,true);
    }
    return 0;
  }

  // if 4 parameters
  string switch2=argv[2];
  string fileLocation=argv[3];
  bool debugMode=false;
  if(switch1=="-l" || switch2=="-l")  
  {
    listInput(fileLocation);
  }
  if(switch1=="-debug" || switch2=="-debug")  
  {
    debugMode=true;
  }
  if(switch1=="-ast" || switch2=="-ast"){
    treePrint(fileLocation,debugMode,true);
  }
  else
    exit(1);
}

void listInput(string fileLocation)   // List the input from the file
{
  string inputLine;
  ifstream fin;
  fin.open(fileLocation.c_str());
  if (fin.is_open())
  {
    while ( getline (fin,inputLine) )
      cout << inputLine << '\n';
    fin.close();
  }
  else 
  {
    cout << "Could not open file"; 
  }
}

void treePrint(string fileLocation, bool debugMode, bool treePrint)     // Produce and print the AST for the input
{
  RPALparser *rp=new RPALparser(fileLocation, debugMode);
  StackHelper *sp=rp->sp;  
  rp->startParsing();
  if(rp->token->nextToken!="")
  {
    cout<<"Could not parse correctly. Input has some elements remaining";
    exit(1);
  }

  Node* t=sp->pop();
  if((sp->pop())!=NULL)
  {
    cout<<"Could not parse correctly. Stack has more than one tree remaining";
  }
  else{
    if(treePrint)
      t->printTree("");
    else{
      t=t->standardizeNode();
      //t->printTree("");
      ControlStructureMaker *csMaker= new ControlStructureMaker(t);
      //csMaker->print();
      CSEMachine *cse=new CSEMachine(csMaker->ls);
      cse->print();
    }  
  }
}