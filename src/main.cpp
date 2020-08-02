#include<iostream>
#include<string>
#include"tinyxml2.h"
#include"Client.h"
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
#include<pthread.h>
#include<atomic>
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
atomic_bool ifcon(false);
void* recvsocket(void* arg)//接受来着客户端数据的线程
{
    int conn = *(int*)arg;
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
    return NULL;
}
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
/*if(roleS == "server"){
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
shared_ptr<Client> c(new Client("clisub_participant"));
c->addTopic("SerCli0","SerCli");
if(roleS == "server"){
	pthread_t tid;
std::cout << "This is server" << std::endl;
    // socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        std::cout << "Error: socket" << std::endl;
        return 0;
    }
    // bind
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
    addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(listenfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        std::cout << "Error: bind" << std::endl;
        return 0;
    }
    // listen
    if(listen(listenfd, 5) == -1) {
        std::cout << "Error: listen" << std::endl;
        return 0;
    }
    // accept
    int conn;
    char clientIP[INET_ADDRSTRLEN] = "";
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
   //bool exitFlag = true;
   ifcon = true;
    while(ifcon){ 
        std::cout << "...listening" << std::endl;
        conn = accept(listenfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
        if (conn < 0) {
            std::cout << "Error: accept" << std::endl;
            continue;
        }
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "...connect " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;
	if(pthread_create(&tid,NULL,recvsocket,(void*)&conn)){
	cout<<"thread create fail"<<endl;
	continue;
	}
        while (ifcon) {
		/*
	int toRec;
	int len = recv(conn,&toRec,sizeof(toRec),0);
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
	    if (strcmp(buf, "exit") == 0) {
                std::cout << "...disconnect " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;
                exitFlag = false;
		string to_send= "aaabbcc";
	    char to_send_size[5];
	    int tss = to_send.size();
	    cout<<"respond int:"<<tss<<" sizeof int "<<sizeof(int)<<endl;
	    memcpy(to_send_size,&tss,sizeof(int));
for (int j = 0; j < 4; j++){
    printf("size content: %x\n", to_send_size[j]);
}
	    send(conn, to_send_size, sizeof(int), 0);
	    send(conn, to_send.c_str(), to_send.size()+1, 0);
		break;
            }
	    std::cout <<"rec content:"<< buf<<std::endl;
            */
string to_send= "aaabbcc";
	    char to_send_size[5];
	    int tss = to_send.size();
	    cout<<"respond int:"<<tss<<" sizeof int "<<sizeof(int)<<endl;
	    memcpy(to_send_size,&tss,sizeof(int));
for (int j = 0; j < 4; j++){
    printf("size content: %x\n", to_send_size[j]);
}
	    if(send(conn, to_send_size, sizeof(int), 0) == -1){
	    cout<<"server send fail"<<endl;
	    break;
	    }
	    if(send(conn, to_send.c_str(), to_send.size()+1, 0) ==-1){
	    cout<<"server send fail"<<endl;
	    break;

	    }
sleep(1);
        }
        
        close(conn);
    }
    close(listenfd);
	
	/*CliWriterListener wl;
	c->addWriter("serpub","SerCli0","SerCli","serclipub0_datawriter",&wl);
CliReaderListener rl;
c->addReader("clisub","SerCli0","SerCli","clisub0_datareader",&rl);
int i = 0;
	while(true){
c->send("serpub",&wl,i);
i++;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}*/
}else{
CliReaderListener rl;
c->addReader("clisub","SerCli0","SerCli","clisub0_datareader",&rl);
}
#endif

return 0;
}
