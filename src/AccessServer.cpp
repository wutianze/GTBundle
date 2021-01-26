#include"AccessServer.h"
bool AccessServer::Accept(){
// accept
    char clientIP[INET_ADDRSTRLEN] = "";
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
   //bool exitFlag = true;
std::cout << "...listening" << std::endl;
int tmp_conn=-1;
while(tmp_conn<0){        
tmp_conn = accept(listenfd_, (struct sockaddr*)&clientAddr, &clientAddrLen);
            //std::cout << "Error: accept" << std::endl;	
	    //sleep(1);
}
cout<<"accept success"<<endl;
conn_ = tmp_conn;
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "...connect " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;
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
	cout<<"AccessServer server recv fail"<<endl;
	this->CloseConnect();
	break;
	}
	cout<<"recv int:"<<toRec<<endl;
	if(toRec > 512){
	cout<<"Warning: may receive a wrong string length, CloseConnect\n";
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
	 cout<<"AccessServer server recv fail"<<endl;
	this->CloseConnect();
	break;
   
	    }
	    bytesLeft-=len;
	    ptr+=len;
	    std::cout<<"recv len:"<<len<<std::endl;
	    buf[toRec] = '\0';
	    }
		//std::cout <<"rec content:"<< buf<<std::endl;
	    if (strcmp(buf, "exit") == 0) {
                std::cout << "...disconnect " << std::endl;

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
	    cout<<"AccessServer::Send, to_send size:"<<tss<<endl;
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
	    cout<<"AccessServer send fail"<<endl;
	    return false;
	    }
return true;
}
bool AccessServer::CloseConnect(){
	cout<<"CloseConnect"<<endl;
ifcon_ = false;
	close(conn_);

return true;
}
bool AccessServer::CloseSocket(){
	cout<<"CloseSocket"<<endl;
		ifcon_ = false;
close(listenfd_);
return true;
}
