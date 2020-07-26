#include<iostream>
#include<string>
#include"tinyxml2.h"
#include"Client.h"
//#include"Server.h"
#include"Listener.h"
#define TEST
using namespace std;
using namespace tinyxml2;
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
CliWriterListener wl;
	c->addWriter("serpub","SerCli0","SerCli","serclipub0_datawriter",&wl);
CliReaderListener rl;
c->addReader("clisub","SerCli0","SerCli","clisub0_datareader",&rl);
int i = 0;
	while(true){
c->send("serpub",&wl,i);
i++;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
}else{
CliReaderListener rl;
c->addReader("clisub","SerCli0","SerCli","clisub0_datareader",&rl);
}
#endif

return 0;
}
