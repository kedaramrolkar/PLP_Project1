#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
using namespace std;

enum TokenType{	isRegularWord, isKeyword, isIdentifier, isString, isInteger };

class Token{
	public:
	string nextToken;
  	TokenType tokenType;
 
  	Token(){
  		tokenType=isRegularWord;
  	}

  	bool isTokenIdentifier(){
  		return tokenType==isIdentifier;
	}

	bool isTokenString(){
		return tokenType==isString;
	}

	bool isTokenInteger(){
	  	return tokenType==isInteger;
	}

	static TokenType IdentifyNextToken(string s)
	{
	    char ch=s[0];
	    if(ch>='0' && ch<='9')
	      return isInteger;
	    
	    if(ch=='"' || ch=='\'')
	    	return isString;
	    
	    if((ch>='A' && ch<='Z') || (ch>='a' && ch<='z'))
	    {
	      if(s=="let" || s=="in" || s=="fn" || s=="where" || s=="aug" || s=="or" || s=="not" || s=="gr" || s=="ge" || s=="ls"
	        || s=="le" || s=="eq" || s=="ne" || s=="true" || s=="false" || s=="nil" || s=="dummy" || s=="within" || s=="and" || s=="rec"){
	        return isKeyword;
	      }
	      return isIdentifier;
	    }
	    return isRegularWord;
  	}

  	string createTokenNode()
  	{
  		string s="";
  		if(isTokenIdentifier() || isTokenString() || isTokenInteger() || 
          nextToken=="true" || nextToken=="false" || nextToken=="nil" || nextToken=="dummy"){
          	if(isTokenIdentifier() || isTokenString() || isTokenInteger()){
          		s=(isTokenIdentifier())?"ID":((isTokenInteger())?"INT":"STR");
          		s=s+":";
          	}
          	s+=nextToken;
          	s="<"+s+">";
    	}
    	return s;
  	}
};