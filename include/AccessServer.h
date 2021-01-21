#ifndef ACCESSERVER_H
#define ACCESSERVER_H
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
#include<map>
#include "Bundle.h"
#include "Listener.h"
using namespace std;
class BunWriter;
class CliReaderListener;
class AccessServer{
private:
       	int listenfd_;
	struct sockaddr_in addr_;
	map<int,int> conn_;
public:
	atomic_bool ifcon_;
	AccessServer(int port){
// socket
    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd_ == -1) {
        std::cout << "Error: socket" << std::endl;
        return;
    }
    int on = 1;
    setsockopt( listenfd_, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on) );
    memset( &addr_, 0, sizeof(addr_) );
    // bind
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    addr_.sin_addr.s_addr = INADDR_ANY;
    if (bind(listenfd_, (struct sockaddr*)&addr_, sizeof(addr_)) == -1) {
        std::cout << "Error: bind" << std::endl;
        return;
    }
    // listen
    if(listen(listenfd_, 5) == -1) {
        std::cout << "Error: listen" << std::endl;
        return;
    }
    cout<<"start listen"<<endl;
    ifcon_ = true;

}
~AccessServer(){
cout<<"delete AccessServer"<<endl;
}
bool Accept(int index);
thread CreateReader(int,BunWriter*,void(*function)(string,BunWriter*));
bool Send(int,string);
bool CloseConnect(int);
bool CloseSocket();
};
#endif
