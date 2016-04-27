#include <cstdlib>
#include <iostream>
#include <string>
#include <stdexcept>
#include <exception>
#include <vector>
#include <math.h> 
#include <unordered_map>
#include "Environment.cpp"
#include "Tokenizer.cpp"
using namespace std;

class CSEMachine
{
 public:
  vector<string> ls;
  Environment *currEnvironment;
  unordered_map <int, unordered_map<string, string>* > hmVar;
  int currEnvCount;
  string currEnvName;
  Tokenizer *tk;
  unordered_map <int, Environment*> hmenv;
  bool debugMode;

  CSEMachine(vector<string> l, bool dm){
    debugMode=dm;
    ls=l;	tk=new Tokenizer();
    currEnvCount=-1;
    currEnvironment=NULL;	currEnvName="";
    start();
  }

  void loadNewEnvironment(Environment *parent, int index){
  	currEnvCount++;
  	if(debugMode)
      cout<<" currEnvCount: "<<currEnvCount<<endl;
  	currEnvironment = new Environment("< e"+to_string(currEnvCount)+" >",parent, debugMode);
  	if(debugMode)
      cout<<" currEnvironment: "<<currEnvironment->name<<endl;

  	unordered_map<int,unordered_map<string, string>*>::const_iterator got = hmVar.find (currEnvCount);
    if ( got != hmVar.end() )
    {
      currEnvironment->hm = got->second; 
    }
    else{
      hmVar.insert(pair<int,unordered_map<string, string>*>(currEnvCount,currEnvironment->hm));
  	}
  	if(debugMode)
      cout<<"hmVar.size:"<<hmVar.size()<<endl;
  	if(hmVar.size()){
	  	if(debugMode)
        cout<<"hmVar.first"<<hmVar.begin(0)->first<<endl;
	  }
  	

  	//hmenv[currEnvCount]=currEnvironment;
  	hmenv.insert(pair<int,Environment*>(currEnvCount,currEnvironment));
  	
  	if(debugMode)
      cout<<"stack to added: "<<currEnvironment->name + " " + ls.at(index)<<endl;
  	if(debugMode)
      cout<<"stack to added: "<<currEnvironment->name<<endl;
  	
  	tk->addToStack(true, currEnvironment->name + " " + ls.at(index));
  	tk->addToStack(false, currEnvironment->name);
    
    //ls.erase(ls.begin());
    currEnvName=currEnvironment->name;
    if(debugMode)
      cout<<"Loaded new Env: "<<endl;
    if(debugMode)
      cout<<"stack:  "+tk->envStack<<endl<<"Tokens: "<<tk->tokenStack<<endl;
  }

  void start()
  {
    loadNewEnvironment(NULL,0);
    if(debugMode)
      cout<<"Start CSE:"<<endl<<endl;
    while(!tk->stackEmpty())
    {
  		if(debugMode)
        cout<<"stack:  "+tk->envStack<<endl<<"Tokens: "<<tk->tokenStack<<endl;
  
    	string s=tk->grabTopToken(true);
    	if(debugMode)
        cout<<endl<<"next Token: ->"+s+"<-"<<endl;

    	if(s==currEnvName)
    	{
    		if(debugMode)
          cout<<"here env: "+currEnvName<<endl;
    		string value=tk->grabTopToken(false);
    		string temp=tk->grabTopToken(false);
    		if(debugMode)
          cout<<value<<" "<<temp<<endl;
    		if(temp!=s){
    			if(debugMode)
            cout<<"env dont match"<<endl;
    			exit(1);
    		}
    		//cout<<"here env: "+currEnvName<<endl;
    		
    		tk->addToStack(false, value);
    		currEnvCount--;
    		if(currEnvCount>=0){
	    		currEnvironment=(Environment*)hmenv[currEnvCount];
	    		currEnvName = currEnvironment->name;
	    		currEnvironment->hm=(unordered_map<string, string>*)hmVar[currEnvCount];
	    		if(debugMode)
            cout<<"loaded env: "+currEnvName<<endl;
    		}
    	}
    	else if(tk->isTokenLambda(s) || tk->isTokenLambdaComma(s))
    	{
			int len=s.length();
    		s.erase(s.end()-1, s.end());
    		s=s+to_string(currEnvCount)+" >";
    		tk->addToStack(false, s);
    	}
    	else if(s=="gamma")
    	{
    		if(tk->isTokenLambda(tk->accessTopToken(false))){
    			if(debugMode)
            cout<<"Gamma-lambda combination"<<endl;
    			string lambda=tk->grabTopToken(false);
    			string trand=tk->grabTopToken(false);
    			if(debugMode)
            cout<<"lambda: "<<lambda<<" trand: "<<trand<<endl;


    			vector<string> x = tk->split(lambda, ' ');
    			string csIndex= x.at(2);	string value= x.at(3);
    			string parentEnv= x.at(4);
    			if(debugMode)
            cout<<"csIndex:"<<csIndex<<" value:"<<value<<" PE:"<<parentEnv<<endl;

    			Environment *parentE=(Environment*)hmenv[stoi( parentEnv )];
    			int CSIndex = stoi( csIndex );
    			if(debugMode)
            cout<<"parent Env: "+parentE->name<<" CSIndex: "<<CSIndex<<endl;
    			loadNewEnvironment(parentE,CSIndex);
    			
    			if(debugMode)
            cout<<"hmVar.first"<<hmVar.begin(currEnvCount)->second->size()<<endl;
    			
    			unordered_map<string, string>::const_iterator got = currEnvironment->hm->find (value);
				  if ( got != currEnvironment->hm->end())
			    {
			    	currEnvironment->hm->erase(value);
			   	}

    			currEnvironment->addItem(value, trand);
    			if(hmVar.size()){
	  				if(debugMode)
              cout<<"hmVar.first"<<hmVar.size()<<endl;
	  				if(debugMode)
              cout<<"hmVar.first"<<hmVar.begin(currEnvCount)->second->size()<<endl;
				  }
    		}
    		else if(tk->isTokenLambdaComma(tk->accessTopToken(false))){
    			if(debugMode)
            cout<<"Gamma-lambda-comma combination"<<endl;
    			string lambda=tk->grabTopToken(false);
    			string trand=tk->grabTopToken(false);
    			if(debugMode)
            cout<<"got: "<<lambda+" "+trand<<endl; 
    			
          if(!tk->isTokenTuple(trand)){
    				if(debugMode)
              cout<<"Not an tuple";
    				exit(1);
    			}

    			if(debugMode)
            cout<<"lambda: "<<lambda<<" trand: "<<trand<<endl;


    			vector<string> x = tk->split(lambda, ' ');
    			int elements= stoi(x.at(2));
    			string csIndex= x.at(3);	
    			string parentEnv= x.at(4+elements);
    			
    			if(debugMode)
            cout<<"E: "<<elements<<" csIndex:"<<csIndex<<" PE:"<<parentEnv<<endl;
    			
    			Environment *parentE=(Environment*)hmenv[stoi( parentEnv )];
    			int CSIndex = stoi( csIndex );
    			if(debugMode)
            cout<<"parent Env: "+parentE->name<<" CSIndex: "<<CSIndex<<endl;
    			loadNewEnvironment(parentE,CSIndex);

    			if(debugMode)
            cout<<"hmVar.first"<<hmVar.begin(currEnvCount)->second->size()<<endl;
    			
    			vector<string> tup = tk->getTupleElements(trand);
    			for(int i=0;i<elements;i++){
    				string tup2=tup.at(i);
    				unordered_map<string, string>::const_iterator got = currEnvironment->hm->find (x.at(4+i));
            if(got != currEnvironment->hm->end())
			    	{
			    		currEnvironment->hm->erase(x.at(4+i));
			   		}
			    	currEnvironment->addItem(x.at(4+i), tup2);
    			}    	

    			if(debugMode)
            cout<<"hmVar.first"<<hmVar.size()<<endl;
	  			if(debugMode)
            cout<<"hmVar.first"<<hmVar.begin(currEnvCount)->second->size()<<endl;		
    		}
    		else if(tk->accessTopToken(false)=="Ystar"){
    			if(debugMode)
            cout<<"Gamma-Ystar combination"<<endl;
    			string Ystar=tk->grabTopToken(false);
    			string lambda=tk->grabTopToken(false);
    			
    			if(debugMode)
            cout<<"got: "<<lambda+" "+Ystar<<endl; 
    			tk->replace(lambda,"lambda","eta");
    			tk->addToStack(false, lambda);
    		}
    		else if(tk->isTokenEta(tk->accessTopToken(false))){
    			if(debugMode)
            cout<<"Gamma-Eta combination"<<endl;
    			string eta=tk->accessTopToken(false);
    			//string lambda=tk->grabTopToken(false);
    			 
    			tk->replace(eta,"eta","lambda");
    			tk->addToStack(false, eta);
    			tk->addToStack(true, "gamma");
    			tk->addToStack(true, "gamma");
    			if(debugMode)
            cout<<"stack:  "+tk->envStack<<endl<<"Tokens: "<<tk->tokenStack<<endl;
  				//cin>>eta;
    		}
    		else if(tk->isTokenTuple(tk->accessTopToken(false)))
    		{
    			if(debugMode)
            cout<<"Gamma-Tau combination"<<endl;
    			string tau=tk->grabTopToken(false);
    			string index2=tk->grabTopToken(false);
    			int i;
    			try{
    				i=stoi(index2);
    			}
    			catch(exception& e){
    				cout<<"ERROR: not an integer, got: "+index2<<endl;
    				exit(1);
    			}

          string element;
          if(i==0)
          {
            element="nil";
          }
          else{

  	   			vector<string> x = tk->getTupleElements(tau);
  	   			try{
      				element= x.at(i-1);
      			}
      			catch(exception& e){
              cout<<"ERROR: range incorrect, got: "<<i<<endl;
      				exit(1);
      			}
          }
	   			tk->addToStack(false, element);
   			}
   			else{
   				if(debugMode)
            cout<<"Gamma-Function combination"<<endl;
    			string function=tk->grabTopToken(false);
    			string value=tk->grabTopToken(false);
    			
    			string result = applyDelayedFunc(function,value);
	   			if(result!="")
            tk->addToStack(false, result);
   			}
    	}
    	else if(tk->isTokenTau(s))
    	{
   			vector<string> x = tk->split(s, ' ');
   			int count= stoi(x.at(2));	
   			if(debugMode)
          cout<<"Tau found, count: "<< count<<endl;
   			string sout="(";
   			for(int i=0;i<count;i++){
   				sout += tk->grabTopToken(false);
   				if(i!=count-1)
   					sout+=", ";
   			}
   			sout+=")";
   			if(debugMode)
          cout<<sout<<endl;
   			tk->addToStack(false, sout);
    	}
    	else if(isTokenKnownFunction(s))
    	{
    		if(debugMode)
          cout<<"known handleFunctions "+s<<endl;
    		handleFunctions(s);
    	}
    	else
    	{
    		if(debugMode)
          cout<<"Last Bucket...currEnv: "+currEnvironment->name+" "<<currEnvCount<<endl;
    		if(currEnvironment->isTokenKnownVariable(s))
    		{
    			s=currEnvironment->getVariableValue(s);
    		}
    		tk->addToStack(false, s);
    	}
    }
  }  

  string processPrint(string value){
  	if(debugMode)
      cout<<"value:"+value<<endl;
  	if(value.at(0)=='\'' && value.at(value.length()-1)=='\'')
  	{
      value = value.substr(1,value.length()-2);
      value=processString(value);
    }
	  else if(tk->isTokenTuple(value))
  		value = processTuple(value);

    else if(tk->isTokenLambda(value))
      value=processLambda(value);
    //printf("%s",value);
  	return value;
  }

  string processString(string str){
    if(str.length()<2)
      return str;

    int i=1;
    char prev=str.at(0);
    string out="";

    bool flag=false;
    for(;i<str.length();i++){
      char newc=str.at(i);
      if(prev=='\\' && newc=='n'){
        out+='\n';
        flag=true;
      }
      else if(prev=='\\' && newc=='t'){
        out+='\t';
        flag=true;
      }
      else if(prev=='n' && newc=='\\'){
        out+='\n';
        flag=true;
      }
      else{
        if(!flag)
          out+=prev;
        else
          flag=false;
      }
      prev=newc;
    }
    if(!flag)
      out+=prev;
    return out;
  }

  string processLambda(string s){
    //cout<<"processing tuple: "<<endl;
    string ans="[lambda closure: ";
    string trand=s;
    vector<string> x = tk->split(trand, ' ');
    ans+=x.at(3)+": "+x.at(2);

    ans+="]";
  return ans;
  }

  string processTuple(string s){
  	//cout<<"processing tuple: "<<endl;
  	string ans="(";
  	string trand=s;
    vector<string> tup = tk->getTupleElements(trand);
    //cout<<" tuple: "+trand<<tup.size()<<endl;
    for(int i=0;i<tup.size();i++){
    	string tup2=tup.at(i);
    	if(debugMode)
        cout<<tup2<<endl;
    	if(tup2.at(0)=='\'' && tup2.at(tup2.length()-1)=='\'')
  		{
        tup2 = tup2.substr(1,tup2.length()-2);
        tup2= processString(tup2);
      }
      ans+=tup2;
    	if(i!=tup.size()-1)
    		ans+=", ";
    }  
    ans+=")";
	return ans;
  }

  void handleFunctions(string token)
  {
  	if(token=="beta"){
    	string firstV=tk->grabTopToken(true);
    	string secondV=tk->grabTopToken(true);
    
    	if(debugMode)
        cout<<"beta"<<firstV<<secondV;
    	vector<string> x = tk->split(firstV, ' ');
    	int deltaElse=stoi(x.at(2));
    	
    	vector<string> y = tk->split(secondV, ' ');
    	int deltaThen=stoi(y.at(2));
    	if(debugMode)
        cout<<"detas:"<<deltaThen<<deltaElse<<endl;
   		string cond=tk->grabTopToken(false);
   		if(debugMode)
        cout<<"--"+cond;
   		if(cond=="true"){
   			tk->addToStack(true, ls.at(deltaThen));
   		}
   		else{
   			tk->addToStack(true, ls.at(deltaElse));
   		}
   		return;
  	}
   	int ans=0;
   	string firstV=tk->grabTopToken(false);
   	if(token=="not"){
   		string ans2=(firstV=="false")?"true":"false";
   		tk->addToStack(false, ans2);
   		return;
   	}
  	else if(token=="neg")
    {
 	  	int ans=0;
 	  	int first = stoi(firstV);
   		ans=first * -1;
    	tk->addToStack(false, to_string(ans));
    	return;
    }
    string secondV=tk->grabTopToken(false);
    
    if(token=="or"){
    	string ans2=(firstV=="true" || secondV=="true")?"true":"false";
    	tk->addToStack(false, ans2);
    	return;
    }
    else if(token=="&"){
    	string ans2=(firstV=="true" && secondV=="true")?"true":"false";
    	tk->addToStack(false, ans2);
    	return;
    }
    else if(token=="aug"){
  		if(!tk->isTokenTuple(firstV)){
  			cout<<"ERROR: Aug with not a tuple";
  			exit(1);
  		}
  		if(firstV=="(nil)" || firstV=="nil")
  		{
  			tk->addToStack(false, "("+secondV+")");
  			return;
  		}

  		firstV[firstV.length()-1]=',';
  		firstV+=" " +secondV +")";
  		tk->addToStack(false, firstV);
  		return;
  	}
   	else if(token=="eq"){
 		string ans2=(firstV==secondV)?"true":"false";
   		tk->addToStack(false, ans2);
   		return;
   	}
   	else if(token=="ne"){
   		string ans2=(firstV!=secondV)?"true":"false";
   		tk->addToStack(false, ans2);
   		return;
   	}
   	int first = stoi(firstV);
  	int second = stoi(secondV);
   	
   	if(token=="ls"){
   		string ans2=(first<second)?"true":"false";
   		tk->addToStack(false, ans2);
   		return;
   	}
   	else if(token=="le"){
   		string ans2=(first<=second)?"true":"false";
   		tk->addToStack(false, ans2);
   		return;
   	}
   	else if(token=="gr"){
   		string ans2=(first>second)?"true":"false";
   		tk->addToStack(false, ans2);
   		return;
   	}
   	else if(token=="ge"){
   		string ans2=(first>=second)?"true":"false";
   		tk->addToStack(false, ans2);
   		return;
   	}
   	if(token=="-")
   		ans=first-second;
   	else if(token=="+")
   		ans=first+second;
   	else if(token=="*")
   		ans=first*second;
   	else if(token=="/")
   		ans=first/second;
   	else if(token=="**")
   		ans=(int)pow((float)first, (float)second) ;
   	tk->addToStack(false, to_string(ans));
   	return;
  }

  bool isTokenKnownFunction(string t){
  	//cout<<"se-"+t+"-"<<endl;
  	if(t=="-" || t=="+" 
  		|| t=="*" || t=="/"
  		|| t=="neg" || t=="**"
  		|| t=="eq" || t=="ne" 
  		|| t=="ls" || t=="le"
  		|| t=="gr" || t=="ge"  
  		|| t=="or" || t=="&" 
  		|| t=="beta" || t=="not" || t=="aug")
  	{
  		return true;
	}
  	return false;
  }

  string applyDelayedFunc(string function, string value){
  	if(function=="Conc"){
    	return "'Conc_"+value.substr(1,value.length()-2)+"'";
  	}
  	else if(function.length() > 5 && function.substr(0,6)=="'Conc_"){
  		return "'"+function.substr(6,function.length()-7)+value.substr(1,value.length()-1);
  	}
  	else if(function=="Print")
  	{
  		cout<<processPrint(value);
    	return "<Dummy>";
  	}
  	else if(function=="Stern")
  	{
  		return "'"+value.substr(2,value.length()-2);
    }
    else if(function=="Stem")
  	{
  		return value.substr(0,2)+"'";
    }
    else if(function=="Order")
  	{
  		if(value=="nil" || value=="(nil)")
  			return to_string(0);

  		vector<string> v=tk->getTupleElements(value);
  		return to_string(v.size());
    }
    else if(function=="Istuple")
  	{
  		return (tk->isTokenTuple(value))?"true":"false";
    }
    else if(function=="Isstring")
  	{
  		return (value.at(0)=='\'' && value.at(value.length()-1)=='\'')?"true":"false";
    }
    else if(function=="Istruthvalue")
    {
      return (value=="true" || value=="false")?"true":"false";
    }
    else if(function=="ItoS")
    {
      try{
        int x=stoi(value);
        value="'"+to_string(x)+"'";
      }
      catch(exception& e){
        cout<<"wrong parameter to ItoS function";
        exit(1);
      }
      return value;
    }
    else if(function=="Isinteger")
    {
      try{
        int x=stoi(value);
        return "true";
      }
      catch(exception& e){
        return "false";
      }
    }
  	else{
  		cout<<"Error: unknown function application, func: "+function;
  		exit(1);
  	}
  }   			
};