#include"AccessServer.h"
int AccessServer::Accept(){
// accept
    char clientIP[INET_ADDRSTRLEN] = "";
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
   //bool exitFlag = true;
   ifcon_ = true;
std::cout << "...listening" << std::endl;
int tmp_conn=-1;
while(tmp_conn<0){        
tmp_conn = accept(listenfd_, (struct sockaddr*)&clientAddr, &clientAddrLen);
            //std::cout << "Error: accept" << std::endl;	
	    //sleep(1);
}
cout<<"accept success"<<endl;
conn_.push_back(tmp_conn);
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "...connect " << clientIP << ":" << ntohs(clientAddr.sin_port) << std::endl;
return conn_.size()-1;
}
thread AccessServer::CreateReader(int index,BunWriter* bw, void(*function)(string,BunWriter*)){
    int tmp_conn = conn_[index];
	return thread([tmp_conn,function,bw]{
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
		//ifcon_ = false;
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
	    if(send(conn, to_send_size, sizeof(int), 0) == -1){
	    cout<<"AccessServer send fail"<<endl;
	    return false;
	    }
	    if(send(conn, to_send.c_str(), to_send.size(), 0) ==-1){
	    cout<<"AccessServer send fail"<<endl;
	    return false;
	    }
return true;
}
bool AccessServer::CloseConnect(int index){
	cout<<"CloseConnect"<<endl;
close(conn_[index]);
return true;
}
bool AccessServer::CloseSocket(){
	cout<<"CloseSocket"<<endl;
close(listenfd_);
return true;
}
