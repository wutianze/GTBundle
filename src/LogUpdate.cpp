#include "LogUpdate.h"
string IAM;
void initLog(){
	IAM = string(getenv("IAM"));
	if(IAM == "CLIENT"){
	cout<<"==========      Client LOG      ==========\n";
	}else{
	cout<<"==========      Server LOG      ==========\n";
	}
}
void logUpdate(string msg, int level){
	if(msg.size()>60)cout<<"log msg too long, will not show\n";
	string to_log = "";
switch(level){
	case Nor:
		to_log = "\r=== "+to_log+"Log: "+msg;
		break;
	case War:
		to_log = "=== "+to_log+"Warning: "+msg;
		break;
	default:
		to_log = "=== "+to_log+"Error: "+msg;
		break;
}
for(int i=to_log.size();i<70;i++){
to_log+=' ';
}
to_log+="===";
switch(level){
	case Nor:
		break;
	default:
		to_log+='\n';
		break;
}
cout<<to_log;
fflush(stdout);
}
