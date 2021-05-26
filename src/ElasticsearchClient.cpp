#include"ElasticsearchClient.h"
Document ElasticsearchClient::getDescription(){
	if(auto res = cli.Get("/")){
	if(res->status == 200){
	Document d;
		d.Parse((res->body).c_str());
		return d;
	}
	}
		return NULL;
	}
Document ElasticsearchClient::getAllIndex(){
	if(auto res = cli.Get("/_cat/indices?v")){
	if(res->status == 200){
	Document d;
		d.Parse((res->body).c_str());
		return d;
	}
	}
		return NULL;
	}

Document ElasticsearchClient::getAllRecord(string index, string type){
	if(auto res = cli.Get(('/'+index+'/'+type).c_str())){
	if(res->status == 200){
	Document d;
		d.Parse((res->body).c_str());
		return d;
	}
	}
		return NULL;
	}

bool ElasticsearchClient::addIndex(string name){
if(auto res = cli.Put(('/'+name).c_str())){
	if(res->status == 200){
	Document d;
		d.Parse((res->body).c_str());
		return d["acknowledged"].GetBool();
	}
	}
		return false;
	}
bool ElasticsearchClient::deleteIndex(string name){
if(auto res = cli.Delete(('/'+name).c_str())){
	if(res->status == 200){
	Document d;
		d.Parse((res->body).c_str());
		return d["acknowledged"].GetBool();
	}
	}
		return false;
	}
