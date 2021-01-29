#include "Bundle.h"
#include <cstdlib>
#define TEST
bool BunWriter::send(){
logUpdate("BunWriter::send: do nothing",Nor);
	return true;
}
bool CliWriter::send(){
	GeneralWriterListener* tmp = dynamic_cast<GeneralWriterListener*>(writer_listener_);	
	if(tmp == nullptr){
	logUpdate("send transfer GeneralWriterListener fail",Err);
	return false;
	}
	if(tmp->matched_>0){
if(!writer_->write(&message_)){
logUpdate("CliWriter send fail",Err);
return false;
}
return true;
	}
return false;
}
bool SerCliWriter::send(){
	GeneralWriterListener* tmp = dynamic_cast<GeneralWriterListener*>(writer_listener_);	
	if(tmp == nullptr){
	logUpdate("send transfer GeneralWriterListener fail",Err);
	return false;
	}
	if(tmp->matched_>0){
if(!writer_->write(&message_)){
logUpdate("SerCliWriter send fail",Err);
return false;
}
return true;
	}
return false;
}
Bundle::Bundle(string config)
	: participant_(nullptr)
{
	logUpdate("new Bundle",Nor);
	mainPath_ = string(getenv("GTPATH"));
	DomainParticipantFactory::get_instance()->load_XML_profiles_file(mainPath_+"/config/profiles.xml"); 
	participant_ = DomainParticipantFactory::get_instance()->create_participant_with_profile(0, config);

	if (participant_ == nullptr)
	{
		logUpdate("participant init fail",Err);
		return;
	}
	types_.insert(pair<string, TypeSupport>("CliSer",new CliSerPubSubType()));
	types_.insert(pair<string, TypeSupport>("SerCli",new SerCliPubSubType()));
	types_["CliSer"].register_type(participant_);
	types_["SerCli"].register_type(participant_);

	publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
	if (publisher_ == nullptr)
	{
		logUpdate("publisher init fail",Err);
		return;
	}
	subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
	if (subscriber_ == nullptr)
	{
		logUpdate("subscriber init fail",Err);
		return;
	}
}
Bundle::~Bundle(){
	logUpdate("delete Bundle",Nor);
	for(map<string, BunReader*>::iterator iter = readers_.begin();iter!= readers_.end();iter++){
		if(iter->second->reader_ != nullptr)subscriber_->delete_datareader(iter->second->reader_);
		delete iter->second->reader_listener_;
		delete iter->second;
		iter->second = nullptr;
	}
	if(subscriber_ != nullptr)participant_->delete_subscriber(subscriber_);
	for(map<string, BunWriter*>::iterator iter = writers_.begin();iter!=writers_.end();iter++){
		if(iter->second->writer_ != nullptr)publisher_->delete_datawriter(iter->second->writer_);
		delete iter->second;
		delete iter->second->writer_listener_;
		iter->second = nullptr;
	}
	if(publisher_ != nullptr)participant_->delete_publisher(publisher_);
	for(map<string, Topic*>::iterator iter = topics_.begin();iter!=topics_.end();iter++){
		if(iter->second != nullptr)participant_->delete_topic(iter->second);
	}
	DomainParticipantFactory::get_instance()->delete_participant(participant_);
	logUpdate("delete Bundle finished",Nor);
}

bool Bundle::addTopic(string topicName, string typeName){
	map<string, TypeSupport>::iterator iter = types_.find(typeName);
	if(iter == types_.end()){
		logUpdate("addTopic Fail: no such type: "+typeName,Err);
		return false;
	}
	Topic* tmp = participant_->create_topic(topicName,(iter->second).get_type_name(), TOPIC_QOS_DEFAULT);
	if(tmp == nullptr){
		logUpdate("addTopic Fail: create_topic: "+topicName+" fail",Err);
		return false;
	}
	pair<map<string, Topic*>::iterator,bool>ret = topics_.insert(pair<string,Topic*>(topicName,tmp));
	if(!ret.second){
	logUpdate("addTopic may insert repeated key",War);
	}
	return true;	
}
bool Bundle::addWriter(string name, string config, DataWriterListener* listener,BunWriter* bunwriter){
	map<string, Topic*>::iterator iter0 = topics_.find(bunwriter->topicName_);
	if(iter0 == topics_.end()){
		logUpdate("addWriter Fail: no such topic: "+bunwriter->topicName_,Err);
		return false;
	}
	map<string, TypeSupport>::iterator iter1 = types_.find(bunwriter->typeName_);
	if(iter1 == types_.end()){
		logUpdate("addWriter Fail: no such type: "+bunwriter->typeName_,Err);
		return false;
	}
	bunwriter->writer_ = publisher_->create_datawriter_with_profile(iter0->second, config, listener);
	if (bunwriter->writer_ == nullptr)
	{
		logUpdate("addWriter Fail: writer init fail",Err);
		return false;
	}
	bunwriter->writer_listener_ = listener;
	pair<map<string, BunWriter*>::iterator,bool>ret = writers_.insert(pair<string, BunWriter*>(name,bunwriter));
	if(!ret.second){
		logUpdate("addWriter Warning: may insert repeated key",War);
	}
	return true;
}
bool Bundle::addReader(string name, string topicName, string typeName, string config, DataReaderListener* listener){
	map<string, Topic*>::iterator iter0 = topics_.find(topicName);
	if(iter0 == topics_.end()){
		logUpdate("addReader Fail: no such topic: "+topicName,Err);
		return false;
	}
	map<string, TypeSupport>::iterator iter1 = types_.find(typeName);
	if(iter1 == types_.end()){
		logUpdate("addReader Fail: no such type: "+typeName,Err);
		return false;
	}
	BunReader* tmp = new BunReader(topicName,typeName);
	tmp->reader_ = subscriber_->create_datareader_with_profile(iter0->second, config, listener);
	if (tmp->reader_ == nullptr)
	{
		logUpdate("addReader Fail: reader init fail",Err);
		return false;
	}
	tmp->reader_listener_ = listener;
	pair<map<string,BunReader*>::iterator,bool>ret = readers_.insert(pair<string, BunReader*>(name,tmp));
	if(!ret.second){
		logUpdate("addReader may insert repeated key, will repalce",War);
		if(subscriber_->delete_datareader((readers_[name])->reader_)!=ReturnCode_t::RETCODE_OK){
		logUpdate("delete old dataReader fail",Err);
		return false;
		}
		//delete readers_[name];//may segmentation fault
		readers_[name] = tmp;
	}
	return true;
}
BunWriter* Bundle::getWriter(string name){
map<string, BunWriter*>::iterator iter = writers_.find(name);
if(iter == writers_.end()){
return nullptr;
}
return iter->second;
}
/*bool Bundle::send(string name,GeneralWriterListener* listener){
	map<string, BunWriter*>::iterator iter = writers_.find(name);
	if(iter == writers_.end()){
		logUpdate("send Fail: no such writer: "<<name<<endl;
		return false;
	}
	if(listener->matched_ > 0){
		iter->second->send();
		logUpdate("successful send"<<endl;
		return true;
	}else{
		logUpdate("send Fail: no matched reader"<<endl;
		return false;
	}
}*/
