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
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include<thread>
#include<atomic>
#include"AccessServer.h"
#include"AccessClient.h"
#define TEST
using namespace std;
using namespace rapidjson;
#define LINKNUM 1
int main(int argc, char** argv){
	string roleS(argv[1]);
	
	if(roleS == "server"){
		cout<<"server here"<<endl;
		Bundle* c=new Bundle("ser_participant");
		GeneralWriterListener* wls[LINKNUM];
		SerCliWriter* sercws[LINKNUM];
		CliSerReaderListener* rls[LINKNUM];
		AccessServer* ass[LINKNUM];
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
		c->addTopic("CliSer"+to_string(ln),"CliSer");
		c->addTopic("SerCli"+to_string(ln),"SerCli");
		wls[ln]=new GeneralWriterListener();
		sercws[ln] = new SerCliWriter("SerCli"+to_string(ln),"SerCli");
		c->addWriter("serclipub","serclipub"+to_string(ln)+"_datawriter",wls[ln],sercws[ln]);
		rls[ln]=new CliSerReaderListener();
		ass[ln]=new AccessServer(8000+ln);
		int to_send_conn = ass[ln]->Accept();
		rls[ln]->setSocketServer(ass[ln]);
		rls[ln]->setSocketTarget(to_send_conn);
		c->addReader("clisersub","CliSer"+to_string(ln),"CliSer","clisersub"+to_string(ln)+"_datareader",rls[ln]);
		rts[ln] = ass[ln]->CreateReader(to_send_conn,c->getWriter("serclipub"),onMessage);
}
for(int ln=0;ln<LINKNUM;ln++)		
{
	rts[ln].join();
		ass[ln]->CloseConnect(0);
}
		delete c;

	}else if(roleS == "client"){
		cout<<"client here"<<endl;
		Bundle* c = new Bundle("cli_participant");
		c->addTopic("SerCli0","SerCli");
		c->addTopic("CliSer0","CliSer");

		GeneralWriterListener* wl=new GeneralWriterListener();
		CliWriter* cliw = new CliWriter("CliSer0","CliSer");
		c->addWriter("clipub","clipub0_datawriter",wl,cliw);
		CliReaderListener*rl=new CliReaderListener();
		
		AccessServer* as=new AccessServer(8000);
		int to_send_conn = as->Accept();
		rl->setSocketServer(as);
		vector<int>targets;
		targets.push_back(to_send_conn);
		rl->setSocketTarget(targets);
		c->addReader("clisub","SerCli0","SerCli","clisub0_datareader",rl);// rl receive from dds server and transfer the msg to java client

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
		thread r0 = as->CreateReader(to_send_conn,c->getWriter("clipub"),onMessage);
		r0.join();

		as->CloseConnect(0);
		cout<<"close connect finished"<<endl;
		delete as;
		delete c;
		cout<<"final delete finished"<<endl;
	}
	return 0;
}
