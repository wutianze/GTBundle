#include"AccessClient.h"
int AccessClient::GetConn(int index){
	if(index >= conn_.size())return -1;
return conn_[index];
}
bool AccessClient::Connect(int index){
if(connect(conn_[index], (struct sockaddr*)&addr_,sizeof(addr_)) < 0) {
        std::cout << "Error: connect" << std::endl;
        return false;
    }

std::cout<<"connect success"<<std::endl;
return true;
}
thread AccessClient::CreateReader(int index,BunWriter* bw, void(*function)(string,BunWriter*)){
    int tmp_conn = conn_[index];
	return thread([tmp_conn,function,bw]{
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
	    function(string(buf),bw);
}	
		});
}
bool AccessClient::Send(int index, string to_send){
    int conn = conn_[index];
	char to_send_size[4];
	    int tss = to_send.size();
	    memcpy(to_send_size,&tss,sizeof(int));
/*for (int j = 0; j < 4; j++){
    printf("size content: %x\n", to_send_size[j]);
}*/
	    if(send(conn, to_send_size, sizeof(int), 0) == -1){
	    cout<<"AccessClient send fail"<<endl;
	    return false;
	    }
	    if(send(conn, to_send.c_str(), to_send.size(), 0) ==-1){
	    cout<<"AccessClient send fail"<<endl;
	    return false;
	    }
return true;
}
bool AccessClient::CloseSocket(int index){
	cout<<"CloseSocket"<<endl;
close(conn_[index]);
return true;
}
