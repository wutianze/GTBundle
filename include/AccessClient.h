#ifndef ACCESCLIENT_H
#define ACCESCLIENT_H
#include<iostream>
#include<string>
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
#include<vector>
#include "Bundle.h"
#include "Listener.h"
using namespace std;
class BunWriter;
class CliReaderListener;
class AccessClient{
private:
       	int clientfd_;
	struct sockaddr_in addr_;
	atomic_bool ifcon_;
	vector<int>conn_;
public:
	AccessClient(string ip_addr, int port){
// socket
    clientfd_ = socket(PF_INET, SOCK_STREAM, 0);
    if (clientfd_ == -1) {
        std::cout << "Error: socket" << std::endl;
        return;
    }
    conn_.push_back(clientfd_);
    memset( &addr_, 0, sizeof(addr_) );
    // bind
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = inet_addr(ip_addr.c_str());
    /*if (inet_pton(AF_INET,ip_,&addr_.sin_addr)<=0) {
        std::cout << "Error: inet_pton" << std::endl;
        return;
    }*/
    // listen
    
    cout<<"start socket client"<<endl;

}
~AccessClient(){
cout<<"delete AccessClient"<<endl;
}
bool Connect(int index);
thread CreateReader(int,BunWriter*,void(*function)(string,BunWriter*));
bool Send(int,string);
bool CloseSocket(int index);
int GetConn(int index);
};
#endif
