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
#include<vector>
#include "Bundle.h"
#include "Listener.h"
#include "LogUpdate.h"
using namespace std;
class BunWriter;
class CliReaderListener;
class AccessServer{
private:
       	int listenfd_;
	struct sockaddr_in addr_;
	int conn_;
public:
	atomic_bool ifcon_;
	AccessServer(int port){
// socket
    listenfd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd_ == -1) {
        logUpdate("socket create fail",Err);
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
        logUpdate("socket bind fail",Err);
        return;
    }
    // listen
    if(listen(listenfd_, 5) == -1) {
        logUpdate("socket listen fail",Err);
        return;
    }
    logUpdate("start listen",Nor);
ifcon_ = true;
}
~AccessServer(){
logUpdate("delete AccessServer",Nor);
}
bool Accept();
thread CreateReader(BunWriter*,void(*function)(string,BunWriter*));
bool Send(string);
bool CloseConnect();
bool CloseSocket();
};
#endif
