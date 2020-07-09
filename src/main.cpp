#include<iostream>
#include<string>
#include"tinyxml2.h"
using namespace std;
using namespace tinyxml2;
int main(int argc, char** argv){
if(argc != 2){
	cout<<"Usage: bundle ../config/default.xml"<<endl;
	return -1;
}
//string cfgFile(argv[1]);

XMLDocument doc;
if(doc.LoadFile(argv[1]) != 0){
fprintf(stderr, "fail to load xml file: %s\n", argv[1]);
		return -1;
}
XMLElement* roleElement = doc.FirstChildElement( "ROLE" );
cout<<"ROLE"<<roleElement->GetText()<<endl;
XMLElement* clientElement = doc.FirstChildElement( "SERVER" )->FirstChildElement( "CLIENT" );
cout<<"CLIENT0"<<clientElement->FirstAttribute()->Next()->Value()<<endl;
clientElement = clientElement->NextSiblingElement();
cout<<"CLIENT1"<<clientElement->FirstAttribute()->Next()->Value()<<endl;
return 0;
}
