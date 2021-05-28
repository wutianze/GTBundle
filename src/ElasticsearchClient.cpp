#include"ElasticsearchClient.h"
#include<iostream>
#include"tools.h"
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
string ElasticsearchClient::getAllIndex(){
	if(auto res = cli.Get("/_cat/indices?v")){
		if(res->status == 200){
			//Document d;
			//d.Parse((res->body).c_str());
			return res->body;
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

// doc must be json
string ElasticsearchClient::addDoc(string index, string doc){
	if(auto res = cli.Post(('/'+index+'/'+"_doc").c_str(),doc,"application/json")){
		if(res->status == 201){
			Document d;
			d.Parse((res->body).c_str());
			return d["_id"].GetString();
		}
	}else{
		auto err = res.error();
		std::cout<<"addDoc error:"<<err<<std::endl;
	}
	return NULL;
}
string ElasticsearchClient::addDoc(string index, Document& doc_d){
	StringBuffer buffer;
	Writer<StringBuffer>writer(buffer);
	doc_d.Accept(writer);
	if(auto res = cli.Post(('/'+index+'/'+"_doc").c_str(),buffer.GetString(),"application/json")){
		if(res->status == 201){
			Document d;
			d.Parse((res->body).c_str());
			return d["_id"].GetString();
		}
	}else{
		auto err = res.error();
		std::cout<<"addDoc error:"<<err<<std::endl;
	}
	return NULL;
}
string ElasticsearchClient::addDocWithTimestamp(string index, Document& doc_d){
	//Value v;
	//v.SetString("fuck",static_cast<SizeType>(4),doc_d.GetAllocator());
	//doc_d.AddMember("ff",v,doc_d.GetAllocator());
	//v.AddMember("time","ff",doc_d.GetAllocator);
	//v.SetString((gmt_time_now()).c_str());
	//doc_d.AddMember("@timestamp",v,doc_d.GetAllocator());
	//doc_d.AddMember(rapidjson::StringRef(key), rapidjson::StringRef(value), doc_d.GetAllocator());
	cout<<gmt_time_now()<<endl;
	char tmp[21];
	strcpy(tmp,gmt_time_now().c_str());
	doc_d.AddMember(rapidjson::StringRef("@timestamp"), rapidjson::StringRef(tmp), doc_d.GetAllocator());
	//doc_d.AddMember(rapidjson::StringRef("@timestamp"), rapidjson::StringRef(gmt_time_now()), doc_d.GetAllocator());
	StringBuffer buffer;
	Writer<StringBuffer>writer(buffer);
	doc_d.Accept(writer);
	cout<<index<<":"<<buffer.GetString()<<endl;
	if(auto res = cli.Post(('/'+index+'/'+"_doc").c_str(),buffer.GetString(),"application/json")){
		if(res->status == 201){
			Document d;
			d.Parse((res->body).c_str());
			return d["_id"].GetString();
		}
	}else{
		cout<<"status:"<<res->status<<endl;
		auto err = res.error();
		std::cout<<"addDoc error:"<<err<<std::endl;
	}
	return NULL;
}
Document ElasticsearchClient::getDoc(string index, string id){
	if(auto res = cli.Get(('/'+index+'/'+"_doc/"+id).c_str())){
		if(res->status == 200){
			cout<<res->body<<endl;
			Document d;
			d.Parse((res->body).c_str());
			return d;
		}
	}else{
		auto err = res.error();
		std::cout<<"getDoc error:"<<err<<std::endl;
	}
	return NULL;
}
