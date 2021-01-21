#include"AccessServer.h"
bool AccessServer::Accept(int index){
// accept
    char clientIP[INET_ADDRSTRLEN] = "";
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
std::cout << "...listening" << std::endl;
int tmp_conn=-1;
while(tmp_conn<0){        
tmp_conn = accept(listenfd_, (struct sockaddr*)&clientAddr, &clientAddrLen);
	    //sleep(1);
}
cout<<"accept success"<<endl;
if(conn_[index] != -1){
cout<<"Warning: AccesssServer Accept an existing index\n";
}
conn_[index] = tmp_conn;
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "...connect " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;
return true;
}
thread AccessServer::CreateReader(int index,BunWriter* bw, void(*function)(string,BunWriter*)){
	return thread([this,index,function,bw]{
	int tmp_conn = this->conn_[index];
			while (true) {
	int toRec;
	int len = recv(tmp_conn,&toRec,sizeof(toRec),0);
	if(len <=0){
	cout<<"AccessServer server recv fail"<<endl;
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
		this->CloseConnect(index);
		break;
        }
	    function(string(buf),bw);
}	
		});
}
bool AccessServer::Send(int index, string to_send){
    int conn = conn_[index];
	char to_send_size[4];
	int tss = to_send.size();
	    cout<<"AccessServer::Send, to_send size:"<<tss<<endl;
	    memcpy(to_send_size,&tss,sizeof(int));
	    //string intString(4,'a');
	    string intString;
	    for(int i=0;i<4;i++){
	    //intString[i] = to_send_size[i];
	    intString.push_back(to_send_size[i]);
	    }
	    string toSend = intString+to_send;
	    //cout<<"intString.size():"<<intString.size()<<",sizeof int:"<<sizeof(int)<<",to_send.size():"<<to_send.size()<<",toSend size:"<<toSend.size()<<",tss:"<<tss<<endl;
	    /*if(send(conn, to_send_size, sizeof(int), 0) == -1){
	    cout<<"AccessServer send fail"<<endl;
	    return false;
	    }*/
	    if(send(conn, toSend.c_str(), toSend.size(), 0) ==-1){
	    cout<<"AccessServer send fail"<<endl;
	    return false;
	    }
return true;
}
bool AccessServer::CloseConnect(int index){
	cout<<"CloseConnect"<<endl;
	if(conn_[index] == -1){
	cout<<"Connection with index:"<<index<<" has been closed before\n";
	return true;
	}
close(conn_[index]);
conn_[index] = -1;
return true;
}
bool AccessServer::CloseSocket(){
	cout<<"CloseSocket"<<endl;
ifcon_ = false;
close(listenfd_);
return true;
}
