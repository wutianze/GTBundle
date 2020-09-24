#include<iostream>
#include<string>
//#include"tinyxml2.h"
#include"Bundle.h"
//#include"Server.h"
#include"Listener.h"
//#include"pistache/endpoint.h"
//#include"json.hpp"
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
/*using namespace Pistache;
using namespace tinyxml2;
struct HelloHandler : public Http::Handler {
  HTTP_PROTOTYPE(HelloHandler)
  void onRequest(const Http::Request&, Http::ResponseWriter writer) override{
    writer.send(Http::Code::Ok, "Hello, World!");
  }
};*/
/*atomic_bool ifcon(false);
void recvsocket(int conn)//接受来着客户端数据的线程
{
while (true) {
	int toRec;
	int len = recv(conn,&toRec,sizeof(toRec),0);
	if(len <=0){
	cout<<"server recv fail"<<endl;
	break;
	}
	cout<<"recv int:"<<toRec<<endl;

	        char buf[toRec+1];
            memset(buf, 0, sizeof(buf));
            int bytesLeft = toRec;
	    char* ptr = buf;
	    while(bytesLeft>0){
	    int len = recv(conn, ptr, bytesLeft, 0);
	    bytesLeft-=len;
	    ptr+=len;
	    std::cout<<"recv len:"<<len<<std::endl;
	    buf[toRec] = '\0';
	    }
		std::cout <<"rec content:"<< buf<<std::endl;
	    if (strcmp(buf, "exit") == 0) {
                std::cout << "...disconnect " << std::endl;
		ifcon = false;
		break;
        }

}
    return;
}*/
int main(int argc, char** argv){
/*char* xmlName;
	if(argc < 2){
		xmlName = "../config/default.xml";
		cout<<"Use default xml file: ../config/default.xml"<<endl;
}else{
xmlName = argv[1];
}
//string cfgFile(argv[1]);

XMLDocument doc;
if(doc.LoadFile(xmlName) != 0){
fprintf(stderr, "fail to load xml file: %s\n", xmlName);
		return -1;
}
XMLElement* roleElement = doc.FirstChildElement( "ROLE" );
cout<<"ROLE"<<roleElement->GetText()<<endl;
XMLElement* clientElement = doc.FirstChildElement( "SERVER" )->FirstChildElement( "CLIENT" );
cout<<"CLIENT0"<<clientElement->FirstAttribute()->Next()->Value()<<endl;
clientElement = clientElement->NextSiblingElement();
cout<<"CLIENT1"<<clientElement->FirstAttribute()->Next()->Value()<<endl;
*/

#ifdef TEST
//Http::listenAndServe<HelloHandler>(Pistache::Address("*:9080"));
//cout<<"http server up"<<endl;
//string roleS(roleElement->GetText());
string roleS(argv[1]);
/*
string clientMode;
if(argc<=2){
clientMode = "sock_server";
}else{
clientMode = string(argv[2]);// sock_server or sock_client
}
if(roleS == "server"){
	cout<<"server run"<<endl;
	shared_ptr<SerCliPub> server(new SerCliPub());
	if(!server->init()){
	cout<<"server init wrong"<<endl;
	return -1;
	}
	server->run();
}else{
	cout<<"client run"<<endl;
	shared_ptr<CliSub> client(new CliSub());
	if(!client->init()){
	cout<<"client init wrong"<<endl;
	return -1;
	}
	while(true){
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
}*/

if(roleS == "server"){
	
cout<<"server here"<<endl;
Bundle* c=new Bundle("ser_participant");
c->addTopic("SerCli0","SerCli");
c->addTopic("CliSer0","CliSer");

GeneralWriterListener* wl=new GeneralWriterListener();
	SerCliWriter* scw = new SerCliWriter("SerCli0","SerCli");
	c->addWriter("serclipub","serclipub0_datawriter",wl,scw);
CliSerReaderListener*rl=new CliSerReaderListener();
c->addReader("clisersub","CliSer0","CliSer","clisersub0_datareader",rl);
(scw->message_).seq(1);
while(!scw->send()){}
(scw->message_).seq(3);
while(!scw->send()){}
while(true){
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
c->addReader("clisub","SerCli0","SerCli","clisub0_datareader",rl);

auto onMessage = [](string msg,BunWriter* bw){
cout<<"in onMessage:"<<msg<<endl;
CliWriter* cw =dynamic_cast<CliWriter*>(bw);
if(cw == nullptr){
cout<<"onMessage Writer transfer wrong"<<endl;
return;
}
Document document;
if(document.Parse(msg.c_str()).HasParseError()){
cout<<"json Parse error"<<endl;
return;
}

(cw->message_).seq(document["seq"].GetInt());
cw->send();
};
thread r0 = as->CreateReader(to_send_conn,c->getWriter("clipub"),onMessage);
//as->Send(0,"1234567");
r0.join();
as->CloseConnect(0);
cout<<"close connect finished"<<endl;
delete as;
delete c;
cout<<"final delete finished"<<endl;
}else{//for test
cout<<"strat test"<<endl;
AccessClient* ac=new AccessClient("10.110.1.199",8090);
ac->Connect(0);
//ac->Send(0,"from client");
int tmp_conn = ac->GetConn(0);
while (true) {
	int toRec;
	int len = recv(tmp_conn,&toRec,sizeof(toRec),0);
	if(len <=0){
	cout<<"server recv fail"<<endl;
	break;
	}
	cout<<"recv int:"<<toRec<<endl;

	        char buf[toRec+1];
            memset(buf, 0, sizeof(buf));
            int bytesLeft = toRec;
	    char* ptr = buf;
	    while(bytesLeft>0){
	    int len = recv(tmp_conn, ptr, bytesLeft, 0);
	    bytesLeft-=len;
	    ptr+=len;
	    std::cout<<"recv len:"<<len<<std::endl;
	    buf[toRec] = '\0';
	    }
		std::cout <<"rec content:"<< buf<<std::endl;
	    if (strcmp(buf, "exit") == 0) {
                std::cout << "...disconnect " << std::endl;
		//ifcon_ = false;
		break;
        }
}
delete ac;
}

#endif

return 0;
}
