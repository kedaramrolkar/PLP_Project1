#include <sstream>
#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
using namespace std;

class Tokenizer
{
 public:
  string envStack;
  string tokenStack;

  Tokenizer(){
  	envStack="";	tokenStack="";
  }

  bool stackEmpty(){
  	return envStack.empty();
  }

  string grabTopToken(bool fromEnvStack){
  	return getNextToken(fromEnvStack, true);
  }

  string accessTopToken(bool fromEnvStack){
  	return getNextToken(fromEnvStack, false);
  }

  void addToStack(bool toEnvStack, string s){
  	if(toEnvStack)
  		envStack+=" " + s;
  	else
  		tokenStack+=" " + s;
  }  

  bool isTokenLambda(string s){
  	if(s.length()>13)
  	if(s.length()>8 && s.substr(0,8)=="< lambda" 
  		&& (s.length()<13 || s.substr(8,5)!="Comma"))
  		return true;
  	return false;
  }

  bool isTokenLambdaComma(string s){
  	if(s.length()>13 && s.substr(0,8)=="< lambda" && s.substr(8,5)=="Comma")
  		return true;
  	return false;
  }

  bool isTokenEta(string s){
  	if(s.length()>5 && s.substr(0,5)=="< eta")
  		return true;
  	return false;
  }

  bool isTokenTau(string s){
  	if(s.length()>5 && s.substr(0,5)=="< tau")
  		return true;
  	return false;
  }

  bool isTokenTuple(string s){
  	if(s=="nil")
  		return true;
  	if(s.at(0)=='(' & s.at(s.length()-1)==')')
  		return true;
  	return false;
  }

  vector<string> &split(const string &s, char delim, vector<string> &elems) 
  {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
  }


  vector<string> split(const string &s, char delim) 
  {
    vector<string> elems;
    split(s, delim, elems);
    return elems;
  }

  vector<string> getTupleElements(string s) 
  {
  	s=s.substr(1,s.length()-2);
    vector<string> elems;
    int start,i=0;
    for(;i<s.length();i++){
      if(s.at(i)==' ')
        i++;
    	start=i;
	    if(s.at(i)=='(')
	    {
	    	int u=1;
	    	i++;
	    	while(i<s.length() && (u>1 || s.at(i)!=')')){
	    		if(s.at(i)=='(')
	    			u++;
	    		if(s.at(i)==')')
	    			u--;
	    		i++;
	    	}
	    	i++;
	    }
	    else{
	    	while(i<s.length() && s.at(i)!=',' )
	    		i++;
	    	if(i>=s.length())
	    		i++;
	    }
      elems.push_back(s.substr(start,i-start));
	  }
    return elems;
  }

  void replace(std::string& str, const std::string& oldStr, const std::string& newStr)
  {
    size_t pos = 0;
    while((pos = str.find(oldStr, pos)) != std::string::npos){
	    str.replace(pos, oldStr.length(), newStr);
	    pos += newStr.length();
	  }
  }

 private:
  string getNextToken(bool fromEnvStack, bool deleteToken){
  	string s="";
  	if(fromEnvStack)
  		s=envStack;
  	else
  		s=tokenStack;

  	string ans="";
  	int len=s.length();
  	int i=len-1;
  	while(s[i]==' ')
  		i--;
  	len=i+1;

  	if(s[i]!='>' && s[i]!=')' && s[i]!='\''){
	  	while(s[i]!=' ' && i>=0)
	  		i--;
  	}
  	else{
  		if(s[i]==')'){
  			int count=1;	i--;
	  		while((count>1 || s[i]!='(') && i>=0){
	  			if(s[i]==')')
	  				count++;
	  			if(s[i]=='(')
	  				count--;
	  			i--;
	  		}
  		}
  		else if(s[i]=='>'){
	  		while(s[i]!='<' && i>=0)
		  		i--;
  		}
  		else{
  			i--;
	  		while(s[i]!='\'' && i>=0)
		  		i--;
  		}
	  	i--;
  	}

  	int j=i+1;
  	while(j<=len-1)
  	{
  		ans+=s[j];
  		j++;
  	}

  	if(deleteToken){
		if(i>-1){
	  		if(fromEnvStack)
	  			envStack.erase(envStack.begin()+i, envStack.end());
	  		else
	  			tokenStack.erase(tokenStack.begin()+i, tokenStack.end());
	  	}
	  	else
	  	{
	  		if(fromEnvStack)
	  			envStack="";
	  		else
	  			tokenStack="";
	  	}
	}
  	return ans;
  }
};