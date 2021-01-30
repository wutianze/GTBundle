#include"AccessServer.h"
bool AccessServer::Accept(){
// accept
    char clientIP[INET_ADDRSTRLEN] = "";
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
   //bool exitFlag = true;
//std::cout << "...listening" << std::endl;
logUpdate("waiting for socket client",Nor);
int tmp_conn=-1;
while(tmp_conn<0){        
tmp_conn = accept(listenfd_, (struct sockaddr*)&clientAddr, &clientAddrLen);
}
logUpdate("accept success",Nor);
conn_ = tmp_conn;
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        logUpdate("...connect "+string(clientIP)+":"+to_string(ntohs(clientAddr.sin_port)),Nor);
	ifcon_ = true;
return true;
}
thread AccessServer::CreateReader(BunWriter* bw, void(*function)(string,BunWriter*)){
	return thread([this,function,bw]{
	//while (this->ifcon_) {
	while(true){
			int toRec;
	int len = recv(this->conn_,&toRec,sizeof(toRec),0);
	if(len <=0){
	logUpdate("AccessServer server recv fail",War);
	this->CloseConnect();
	break;
	}
	//cout<<"recv int:"<<toRec<<endl;
	if(toRec > 512){
	logUpdate("may receive a wrong string length, CloseConnect",War);
	this->CloseConnect();
	break;
	}

	        char buf[toRec+1];
            memset(buf, 0, sizeof(buf));
            int bytesLeft = toRec;
	    char* ptr = buf;
	    while(bytesLeft>0){
	    int len = recv(this->conn_, ptr, bytesLeft, 0);
	    if(len<=0){
	 logUpdate("AccessServer server recv fail",War);
	this->CloseConnect();
	break;
   
	    }
	    bytesLeft-=len;
	    ptr+=len;
	    buf[toRec] = '\0';
	    }
	    if (strcmp(buf, "exit") == 0) {
                logUpdate("...disconnect socket",Nor);

	this->CloseConnect();
		break;
        }
	    function(string(buf),bw);
}	
		});
}
bool AccessServer::Send(string to_send){
	char to_send_size[4];
	int tss = to_send.size();
	    logUpdate("Msg sized: "+to_string(tss)+" Bundle ===> Socket",Nor);
	    memcpy(to_send_size,&tss,sizeof(int));
	    string intString;
	    for(int i=0;i<4;i++){
	    intString.push_back(to_send_size[i]);
	    }
	    string toSend = intString+to_send;
	    while(!ifcon_){
	    sleep(10);
	    }
	    if(send(conn_, toSend.c_str(), toSend.size(), 0) ==-1){
	    logUpdate("AccessServer send fail",War);
	    return false;
	    }
return true;
}
bool AccessServer::CloseConnect(){
	logUpdate("Close Socket Connect",Nor);
ifcon_ = false;
	close(conn_);

return true;
}
bool AccessServer::CloseSocket(){
	logUpdate("CloseSocket",Nor);
		ifcon_ = false;
close(listenfd_);
return true;
}
