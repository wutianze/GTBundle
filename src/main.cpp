#include<iostream>
#include<string>
#include"tinyxml2.h"
#include"Client.h"
#include"Server.h"
#define TEST
using namespace std;
using namespace tinyxml2;
int main(int argc, char** argv){
char* xmlName;
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

#ifdef TEST
string roleS(roleElement->GetText());
if(roleS == "server"){
	cout<<"server run"<<endl;
	shared_ptr<CliSerSub> server(new CliSerSub());
	if(!server->init()){
	cout<<"server init wrong"<<endl;
	return -1;
	}
	server->run();
}else{
	cout<<"client run"<<endl;
	shared_ptr<CliPub> client(new CliPub());
	if(!client->init()){
	cout<<"client init wrong"<<endl;
	return -1;
	}
	client->run();
}
#endif

return 0;
}
