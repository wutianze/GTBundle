#include<iostream>
#include<string>
#include"Bundle.h"
#include"Listener.h"
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
/*#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"*/
#include<thread>
#include<atomic>
#include<signal.h>
#include"AccessServer.h"
using namespace std;
using namespace rapidjson;
AccessServer* as = new AccessServer(8000);
void exit_func(int signum){
as->CloseSocket();
}

int main(int argc, char** argv){
	signal(SIGINT,exit_func);
	string roleS(argv[1]);
	if(roleS == "server"){
		int LINKNUM = atoi(getenv("LINKNUM"));
		cout<<"server here"<<endl;
		Bundle* c=new Bundle("ser_participant");
		GeneralWriterListener* wls[LINKNUM];
		SerCliWriter* sercws[LINKNUM];
		CliSerReaderListener* rls[LINKNUM];
		thread rts[LINKNUM];
		auto onMessage = [](string msg,BunWriter* bw){
			cout<<"receive from java client in Controller side:"<<msg<<endl;
			SerCliWriter* scw =dynamic_cast<SerCliWriter*>(bw);
			if(scw == nullptr){
				cout<<"onMessage Writer transfer wrong"<<endl;
				return;
			}
			(scw->message_).seq(1);
			(scw->message_).com(msg);// content is the GeneratorJSON
			scw->send();
		};

for(int ln=0;ln<LINKNUM;ln++){
		if(!c->addTopic("CliSer"+to_string(ln),"CliSer")){
		return -1;
		}
		if(!c->addTopic("SerCli"+to_string(ln),"SerCli")){
		return -1;
		}
		wls[ln]=new GeneralWriterListener();
		sercws[ln] = new SerCliWriter("SerCli"+to_string(ln),"SerCli");
		if(!c->addWriter("serclipub"+to_string(ln),"serclipub"+to_string(ln)+"_datawriter",wls[ln],sercws[ln])){
		return -1;
		}
		rls[ln]=new CliSerReaderListener();
		rts[ln] = thread([&as,&rls,&c,ln,onMessage]{
		while(as->ifcon_){
		int to_send_conn = as->Accept(ln);
		rls[ln]->setSocketServer(as);
		rls[ln]->setSocketTarget(ln);
		if(!c->addReader("clisersub"+to_string(ln),"CliSer"+to_string(ln),"CliSer","clisersub"+to_string(ln)+"_datareader",rls[ln])){
		return -1;
		}
		thread tmpThread = as->CreateReader(ln,c->getWriter("serclipub"+to_string(ln)),onMessage);	
		tmpThread.join();
		}
				});
		
}
for(int ln=0;ln<LINKNUM;ln++)		
{
	rts[ln].join();
		as->CloseConnect(ln);
}
as->CloseSocket();
		delete c;
		delete as;

	}else if(roleS == "client"){
		cout<<"client here"<<endl;
		string GLOBAL_INDEX = string(getenv("GLOBAL_INDEX"));
		Bundle* c = new Bundle("cli_participant");
		if(!c->addTopic("SerCli"+GLOBAL_INDEX,"SerCli")){
		return -1;
		}
		if(!c->addTopic("CliSer"+GLOBAL_INDEX,"CliSer")){
		return -1;
		}
auto onMessage = [](string msg,BunWriter* bw){
			cout<<"received from local java client in Generator side:"<<msg<<endl;
			CliWriter* cw =dynamic_cast<CliWriter*>(bw);
			if(cw == nullptr){
				cout<<"onMessage Writer transfer wrong"<<endl;
				return;
			}
			(cw->message_).seq(0);// no use currently
			(cw->message_).com(msg);// content is the GeneratorJSON
			cw->send();
		};
		GeneralWriterListener* wl=new GeneralWriterListener();
		CliWriter* cliw = new CliWriter("CliSer"+GLOBAL_INDEX,"CliSer");
		if(!c->addWriter("clipub","clipub"+GLOBAL_INDEX+"_datawriter",wl,cliw)){
		return -1;
		}
		CliReaderListener*rl=new CliReaderListener();
	while(as->ifcon_){	
		int to_send_conn = as->Accept(0);
		rl->setSocketServer(as);
		rl->setSocketTarget(0);
		if(!c->addReader("clisub","SerCli"+GLOBAL_INDEX,"SerCli","clisub"+GLOBAL_INDEX+"_datareader",rl)){
		return -1;
		}// rl receive from dds server and transfer the msg to java client
		
		thread r0 = as->CreateReader(to_send_conn,c->getWriter("clipub"),onMessage);
		r0.join();
	}
		as->CloseConnect(0);
		as->CloseSocket();
		delete as;
		delete c;
	}
	return 0;
}
