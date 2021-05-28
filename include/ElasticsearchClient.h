#ifndef ELASTICSEARCH_H
#define ELASTICSEARCH_H
#include "httplib.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <string>
using namespace std;
using namespace rapidjson;
using namespace httplib;
class ElasticsearchClient{
	public:
		httplib::Client cli;
ElasticsearchClient(string address="http://localhost:9200"):cli(address.c_str()){};
	
	Document getDescription();
	string getAllIndex();
	Document getAllRecord(string index, string type);

	bool addIndex(string name);
	bool deleteIndex(string name);

	string addDoc(string index, string doc);
	string addDoc(string index, Document& doc_d);
	string addDocWithTimestamp(string index, Document& doc_d);
	Document getDoc(string index, string id);
};

#endif
