#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include "StackHelper.cpp"
#include "LexicalAnalyser.cpp"
using namespace std;

class RPALparser{
  private:
  void E(){
    if(token->nextToken=="let")
    {
      readNextToken("let");
      print("\nD called from E");
      D();
      print("\nD recalled from E");
      readNextToken("in");
      print("\nE called from E");
      E();
      print("\nE recalled from E");
      sp->buildTree("let",2);
    }
    else if(token->nextToken=="fn")
    {    
      readNextToken("fn");
      int n=0;
      do{
        print("\nVb called from E");
        Vb(); n++;
        print("\nVb recalled from E");
      }while(token->isTokenIdentifier() || token->nextToken=="(");
      
      readNextToken(".");
      print("\nE called from E");
      E();
      print("\nE recalled from E");
      sp->buildTree("lambda",n+1);
    }  
    else{
      print("\nEw called from E");
      Ew();
      print("\nEw recalled from E");
    }
  }
  
  void Ew(){
    print("\nT called from Ew");
    T();
    print("\nT recalled from Ew");
    if(token->nextToken=="where"){
      readNextToken("where");
      print("\nDr called from Ew");
      Dr();
      print("\nDr recalled from Ew");
      sp->buildTree("where",2);
    }
  }
  
  void T(){
    print("\nTa called from T");
    Ta();
    print("\nTa recalled from T");
    int n=1;
    while(token->nextToken==","){
      readNextToken(",");
      print("\nTa called from T");
      Ta();
      print("\nTa recalled from T");
      n++;
    }
    if(n>1)
      sp->buildTree("tau",n);
  }
  
  void Ta(){
    print("\nTc called from Ta");
    Tc();
    print("\nTc recalled from Ta");
    while(token->nextToken=="aug"){
      readNextToken("aug");
      print("\nTc called from Ta");
      Tc();
      print("\nTc recalled from Ta");
      sp->buildTree("aug",2);
    }
  }
  
  void Tc(){
    print("\nB called from Tc");
    B();
    print("\nB recalled from Tc");
    if(token->nextToken=="->")
    {
      readNextToken("->");
      print("\nTc called from Tc");
      Tc();
      print("\nTc recalled from Tc");
      readNextToken("|");
      print("\nTc called from Tc");
      Tc();
      print("\nTc recalled from Tc");
      sp->buildTree("->",3);
    }  
  }
  
  void B(){
    print("\nBt called from B");
    Bt();
    print("\nBt recalled from B");
    while(token->nextToken=="or"){
      readNextToken("or");
      print("\nBt called from B");
      Bt();
      print("\nBt recalled from B");    
      sp->buildTree("or",2);
    }
  }
  
  void Bt(){
    print("\nBs called from Bt");
    Bs();
    print("\nBs recalled from Bt");
    while(token->nextToken=="&"){
      readNextToken("&");
      print("\nBs called from Bt");
      Bs();
      print("\nBs recalled from Bt");
      sp->buildTree("&",2);
    }
  }
  
  void Bs(){
    if(token->nextToken=="not"){
      readNextToken("not");
      print("\nBp called from Bs");
      Bp();
      print("\nBp recalled from Bs");
      sp->buildTree("not",1);
    }
    else{
      print("\nBp called from Bs");
      Bp();
      print("\nBp recalled from Bs");
    }
  }
  
  void Bp(){
    print("\nA called from Bp");
    A();
    print("\nA recalled from Bp");
    if(token->nextToken=="gr" || token->nextToken==">"){
        readNextToken(token->nextToken);
        print("\nA called from Bp");
        A();
        print("\nA recalled from Bp");
        sp->buildTree("gr",2);
    }
    else if(token->nextToken=="ge" || token->nextToken==">=")
    {
        readNextToken(token->nextToken);
        print("\nA called from Bp");  
        A();
        print("\nA recalled from Bp");
        sp->buildTree("ge",2);
    }
    else if(token->nextToken=="ls" || token->nextToken=="<")
    {
      readNextToken(token->nextToken);
      print("\nA called from Bp");
      A();
      print("\nA recalled from Bp");
      sp->buildTree("ls",2);
    }
    else if(token->nextToken=="le" || token->nextToken=="<=")
    {  
      readNextToken(token->nextToken);
      print("\nA called from Bp");  
      A();
      print("\nA recalled from Bp");
      sp->buildTree("le",2);
    }
    else if(token->nextToken=="eq" || token->nextToken=="ne")
    {
      string temp=token->nextToken;
      readNextToken(temp);
      print("\nA called from Bp");
      A();
      print("\nA recalled from Bp");
      sp->buildTree(temp,2);
    }
  }
  
  void A()
  {
    if(token->nextToken=="-")
    {
      readNextToken("-");
      print("\nAt called from A");
      At();
      print("\nAt recalled from A");
      sp->buildTree("neg",1);
    }
    else if(token->nextToken=="+"){
      readNextToken("+");
      print("\nAt called from A");
      At();
      print("\nAt recalled from A");
    }
    else  {
      print("\nAt called from A");
      At();
      print("\nAt recalled from A");
    }
  
    while(token->nextToken=="-" || token->nextToken=="+"){
      string temp=token->nextToken;
      readNextToken(token->nextToken);
      print("\nAt called from A");
      At();
      print("\nAt recalled from A");
      sp->buildTree(temp,2);
    }
  }
  
  void At(){
    print("\nAf called from At");
    Af();
    print("\nAf recalled from At");
    
    while(token->nextToken=="*" || token->nextToken=="/"){
      string temp=token->nextToken;
      readNextToken(temp);
      print("\nAf called from At");
      Af();
      print("\nAf recalled from At");
      sp->buildTree(temp,2);
    }
  }
  
  void Af()
  {
    print("\nAp called from Af");
    Ap();
    print("\nAp recalled from Af");
    if(token->nextToken=="**"){
      readNextToken("**");
      print("\nAp called from Af");
      Af();
      print("\nAp recalled from Af");
      sp->buildTree("**",2);
    }
  } 

  void Ap(){
    print("\nR called from Ap");
    R();
    print("\nR recalled from Ap");
    while(token->nextToken=="@"){
      readNextToken("@");
      if(!(token->isTokenIdentifier())){
        cout<<"Error: Incorrect token in Ap. expected 'Identifier'";
        exit(1);
      }
      readNextToken(token->nextToken);
      
      print("\nR called from Ap");
      R();
      print("\nR recalled from Ap");
      sp->buildTree("@",3);
    }
  }
  
  void R(){
    print("\nRn called from R");
    Rn();
    print("\nRn recalled from R");
    
    while(token->isTokenIdentifier() || token->isTokenString() || token->isTokenInteger() || token->nextToken=="true" 
            || token->nextToken=="false" || token->nextToken=="nil" || token->nextToken=="(" || token->nextToken=="dummy")
    {
      string temp=token->nextToken;
      print("\nRn called from R");
      Rn();
      print("\nRn recalled from R");
      sp->buildTree("gamma",2);
    }
  }
  
  void Rn(){
    if(token->isTokenIdentifier() || token->isTokenInteger() || token->isTokenString() || token->nextToken=="true" ||
      token->nextToken=="false" || token->nextToken=="nil" || token->nextToken=="dummy"){
        readNextToken(token->nextToken);
    } 
    else if(token->nextToken=="("){
      readNextToken("(");
      print("\nE called from Rn");  
      E();
      print("\nE recalled from Rn");
      readNextToken(")");
    }
    else  
    {
      cout<<"Error: Invalid Token in Rn.";
      exit(1);
    } 
  }
  
  void D(){
    print("\nDa called from D");
    Da();
    print("\nDa recalled from D");
    if(token->nextToken=="within"){
      readNextToken("within");
      print("\nDa called from D");
      Da();
      print("\nDa recalled from D");
      sp->buildTree("within",2);
    }
  }
  
  void Da(){
    print("\nDr called from Da");
    Dr();
    print("\nDr recalled from Da");
    int n=1;
    while(token->nextToken=="and"){
      readNextToken("and");
      print("\nDr called from Da");
      Dr(); 
      n++;
      print("\nDr recalled from Da");
    };
    if(n>1)
      sp->buildTree("and",n);
  }
  
  void Dr(){
    print("\nDb called from Dr");
    if(token->nextToken=="rec"){
      readNextToken("rec");
      Db();
      sp->buildTree("rec",1);
    }
    else{
      Db();
    }
    print("\nDb recalled from Dr");
  }
  
  void Db(){
    if(token->nextToken=="("){
      readNextToken("(");
      print("\nD called from Db");
      D();
      print("\nD recalled from Db");
      readNextToken(")");
    }
    else if(token->isTokenIdentifier()){
      readNextToken(token->nextToken);
      int n=1;
      if (token->nextToken=="(" || token->isTokenIdentifier())
      {
        while(token->nextToken=="(" || token->isTokenIdentifier())
        {
          print("\nVb called from Db");
          Vb();
          n++;
          print("\nVb recalled from Db");
        }
        readNextToken("=");
        print("\nE called from Db");
        E();
        print("\nE recalled from Db");
        sp->buildTree("function_form",n+1);
      }
      else if(token->nextToken=="," || token->nextToken=="="){
        if(token->nextToken==","){
          n=n+Vl();
          sp->buildTree(",",n);
        }
        readNextToken("=");
        print("\nE called from Db");
        E();
        print("\nE recalled from Db");
        sp->buildTree("=",2);
      }
      else{
        cout<<"Error: Incorrect token after the 'Identifier' for Db.";
        exit(1);
      }
    }
    else{
      cout<<"Error: Incorrect starting token for Db. Expected was '(' or 'Identifier'";
      exit(1);
    }
  }
  
  void Vb(){
    if(token->isTokenIdentifier()){
      readNextToken(token->nextToken);
    }
    else if(token->nextToken=="("){
      readNextToken("(");
      if(token->nextToken==")"){
        readNextToken(")");
        sp->buildTree("()",0);
      }
      else{
        if(token->isTokenIdentifier()){
          readNextToken(token->nextToken);
          int n=1;
          if(token->nextToken==","){
            print("\nVl called from Vb");  
            n=n+Vl();
            print("\nVl recalled from Vb");
            sp->buildTree(",",n);
          }
        }
        else{
          cout<<"Error: Incorrect token for Vl in Vb. Only allowed 'id'";
          exit(1);
        }
        readNextToken(")");
      }
    }
    else{
      cout<<"Error: Incorrect token for Vb. Only allowed is 'Identifier' or '('";
      exit(1);
    }
  }
  
  int Vl(){
    int n=0;
    if(token->nextToken==",")
    {
      do{
        readNextToken(",");
        if(!token->isTokenIdentifier()){
          cout<<"Error: Incorrect token in Vl. Expected 'id'";
          exit(1);
        }
        readNextToken(token->nextToken);
        n++;
      }while(token->nextToken==",");
    }
    else{
      cout<<"Error: Incorrect token for Vl. Only allowed ','";
      exit(1);
    }
    return n;
  }
  
  void grabNextToken(){
    la->grabNextToken();
  }

  void readNextToken(string expectedToken){
    if(expectedToken!=token->nextToken){
      cout<<"Error: Incorrect token read. Expected was '"<<expectedToken<<"'";
      exit(1);
    }
    
    string nodeValue=token->createTokenNode();
    if(nodeValue!="")
      sp->buildTree(nodeValue,0);
    grabNextToken();
  }

  void print(string s){
    if(debugMode)
      cout<<s;
  }

public:
  StackHelper *sp;
  LexicalAnalyser *la;
  Token *token;
  bool debugMode;

  RPALparser(string filelocation, bool dm){
    debugMode=dm;
    token=new Token();
    sp=new StackHelper(dm);
    la=new LexicalAnalyser(filelocation.c_str(), token, dm);
  }
  
  void startParsing()
  {
    E();
  }
};


