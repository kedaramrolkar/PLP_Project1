#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include "Token.cpp"
using namespace std;

class LexicalAnalyser
{
 public:                    // StackNode will form the basic nodes for the stack
  ifstream fin;
  char c;
  Token *token;
  bool debugMode;

  LexicalAnalyser(string fl, Token *t, bool dm){
  	debugMode = dm;
  	token=t;
    fin.open(fl.c_str());
    	if(!fin.good()){
      		cout<<"Error: File '" << fl<<"' could not be found.";
  	 		exit(1);
  	 	}
    c='\0';
    grabNextToken();
  }

  void grabNextToken(){
    string str="";
    if(fin.eof() || c==EOF){
    	token->nextToken=""; 	token->tokenType=isRegularWord;
    	return;
    }

    while(c==' ' || c=='\0' || c==9 || c==4 || c=='\n')
    {
		if(fin.eof())
		{
		  token->nextToken=""; 	token->tokenType=isRegularWord;
		  return;
		}
		fin.get(c);
    }

    bool flag=false;
    while(1){
      if(c==' ' || c==9 || c==4 || c=='\0' || fin.eof() || c=='\n') //white spaces
        break;
      if(c=='(' || c==')')  //brackets
        break;
      if(c=='+' || c=='&' || c=='=')  //additions
        break;
      if(c=='.' || c==',' || c=='@')  //dot
        break;
      if(c=='|')  
        break;
      
      if(c=='<' || c=='>')
      {
        if(flag)
        	break;
        else
        {
		  	str=c;
		  	fin.get(c);
	        if(c =='='){
	          str+=c;
	          break;
	        }
		  	flag=true;
	        break;
        }
      }
      if(c=='-'){
        if(flag)
          break;
        else
        {
		  str=c;
		  fin.get(c);
	      if(c=='>')
	      	break;
		  flag=true;
	      break;
	    }
      }
      if(c=='*'){
        if(flag)
          break;
        else
        {
		  str=c;
		  fin.get(c);
	      if(c=='*'){
	        break;
	      }
		  flag=true;
	      break;
	    }
      }
      if(c=='/'){
        if(flag)
          break;
        
        else{
		  str=c;
		  fin.get(c);
	      if(c=='/'){
	        while(c!='\n')
	      	 	fin.get(c);
	        while(c==' ' || c==9 || c==4 || c=='\n')
	        	fin.get(c);
	        str="";
	        continue;
	      }
	      else{
		    flag=true;
	        break;
		  }
	    }
      }
      if(c=='"'){
        if(flag){
          cout<<"unexpected \" occurred";
          return;
        }
        else{
	  	  str=c;
          fin.get(c);
          while(c!='"'){
	    	str+=c;
            if(c=='\\'){
              	fin.get(c);
	      		str+=c;
	        }
          	fin.get(c);
          }
          break;
        }
      }
      if(c=='\''){
        if(flag){
          cout<<"unexpected \' occurred";
          return;
        }
        else{
          str=c;
          fin.get(c);
          while(c!='\''){
            str+=c;
            if(c=='\\'){
              	fin.get(c);
	      		str+=c;
	    	}
            fin.get(c);
          }
          //str+=c;
          break;
        }
      }
      
      flag=true;
      str=str+c;
      fin.get(c);
    }        
    if(!flag){    
      str+=c;
      c='\0';
    }
    
    if(debugMode)
    	cout<<"\ngrabbed next token: '"<<str<<"'";
    token->nextToken=str; 	token->tokenType=token->IdentifyNextToken(str);
  }
};