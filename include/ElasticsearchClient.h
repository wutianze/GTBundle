#ifndef ELASTICSEARCH_H
#define ELASTICSEARCH_H
#endif
#include "httplib.h"
#include "rapidjson/document.h"
#include <string>
using namespace std;
using namespace rapidjson;
class ElasticsearchClient{
	public:
		httplib::Client cli;
ElasticsearchClient(string address="http://localhost:9200"):cli(address.c_str()){};
	
	Document getDescription();
	Document getAllIndex();
	Document getAllRecord(string index, string type);

	bool addIndex(string name);
	bool deleteIndex(string name);
};
