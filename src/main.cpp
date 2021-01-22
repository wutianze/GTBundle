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
#include<thread>
#include<atomic>
#include <signal.h>
#include"AccessServer.h"
using namespace std;
int LINKNUM=1;// 1 is for client
AccessServer** ass;
void exit_func(int signum){
	for(int i=0;i<LINKNUM;i++){
		ass[i]->CloseSocket();
	}
}
int main(int argc, char** argv){
	//signal(SIGINT,exit_func);
	string roleS(argv[1]);
	if(roleS == "server"){
		LINKNUM = atoi(getenv("LINKNUM"));
		cout<<"server here"<<endl;
		Bundle* c=new Bundle("ser_participant");
		GeneralWriterListener* wls[LINKNUM];
		SerCliWriter* sercws[LINKNUM];
		CliSerReaderListener* rls[LINKNUM];
		ass = new AccessServer*[LINKNUM];
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
			ass[ln]=new AccessServer(8000+ln);
			rls[ln]->setSocketServer(ass[ln]);
			wls[ln]->setSocketServer(ass[ln]);

			rts[ln] = thread([ass,&rls,&c,ln,onMessage]{
					//while(ass[ln]->ifcon_){
					while(true){
					ass[ln]->Accept();
					if(!c->addReader("clisersub"+to_string(ln),"CliSer"+to_string(ln),"CliSer","clisersub"+to_string(ln)+"_datareader",rls[ln])){
					return -1;
					}

					thread tmpThread = ass[ln]->CreateReader(c->getWriter("serclipub"+to_string(ln)),onMessage);	
					tmpThread.join();
					}
					});

			}
			for(int ln=0;ln<LINKNUM;ln++)		
			{
				rts[ln].join();
				ass[ln]->CloseConnect();
				ass[ln]->CloseSocket();
				delete ass[ln];
			}
			delete ass;
			delete c;


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

			GeneralWriterListener* wl=new GeneralWriterListener();
			CliWriter* cliw = new CliWriter("CliSer"+GLOBAL_INDEX,"CliSer");
			if(!c->addWriter("clipub","clipub"+GLOBAL_INDEX+"_datawriter",wl,cliw)){
				return -1;
			}
			CliReaderListener*rl=new CliReaderListener();
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

			ass = new AccessServer*[1];
			ass[0] = new AccessServer(8000);
			rl->setSocketServer(ass[0]);
			wl->setSocketServer(ass[0]);

			//while(ass[0]->ifcon_){
			while(true){
				ass[0]->Accept();
				if(!c->addReader("clisub","SerCli"+GLOBAL_INDEX,"SerCli","clisub"+GLOBAL_INDEX+"_datareader",rl)){
					return -1;
				}// rl receive from dds server and transfer the msg to java client

				thread r0 = ass[0]->CreateReader(c->getWriter("clipub"),onMessage);
				r0.join();
			}

			ass[0]->CloseConnect();
			ass[0]->CloseSocket();
			delete ass[0];
			delete ass;
			delete c;
		}
		return 0;
		}
