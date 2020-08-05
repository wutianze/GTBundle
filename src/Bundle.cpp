#include "Bundle.h"
#include <cstdlib>
#define TEST
bool BunWriter::send(){
cout<<"BunWriter::send: do nothing"<<endl;
	return true;
}
bool CliWriter::send(){
writer_->write(&message_);
return true;
}

Bundle::Bundle(string config)
	: participant_(nullptr)
{
	cout<<"new Bundle"<<endl;
	mainPath_ = string(getenv("GTPATH"));
	DomainParticipantFactory::get_instance()->load_XML_profiles_file(mainPath_+"/config/profiles.xml"); 
	participant_ = DomainParticipantFactory::get_instance()->create_participant_with_profile(0, config);

	if (participant_ == nullptr)
	{
		cout<<"participant init fail"<<endl;
		return;
	}
	types_.insert(pair<string, TypeSupport>("CliSer",new CliSerPubSubType()));
	types_.insert(pair<string, TypeSupport>("SerCli",new SerCliPubSubType()));
	types_["CliSer"].register_type(participant_);
	types_["SerCli"].register_type(participant_);

	publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
	if (publisher_ == nullptr)
	{
		cout<<"publisher init fail"<<endl;
		return;
	}
	subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);
	if (subscriber_ == nullptr)
	{
		cout<<"subscriber init fail"<<endl;
		return;
	}
}
Bundle::~Bundle(){
	cout<<"delete Bundle"<<endl;
	for(map<string, BunReader*>::iterator iter = readers_.begin();iter!= readers_.end();iter++){
		if(iter->second->reader_ != nullptr)subscriber_->delete_datareader(iter->second->reader_);
		delete iter->second;
		iter->second = nullptr;
	}
	if(subscriber_ != nullptr)participant_->delete_subscriber(subscriber_);
	for(map<string, BunWriter*>::iterator iter = writers_.begin();iter!=writers_.end();iter++){
		if(iter->second->writer_ != nullptr)publisher_->delete_datawriter(iter->second->writer_);
		delete iter->second;
		iter->second = nullptr;
	}
	if(publisher_ != nullptr)participant_->delete_publisher(publisher_);
	for(map<string, Topic*>::iterator iter = topics_.begin();iter!=topics_.end();iter++){
		if(iter->second != nullptr)participant_->delete_topic(iter->second);
	}
	DomainParticipantFactory::get_instance()->delete_participant(participant_);
	cout<<"delete Bundle finished"<<endl;
}

bool Bundle::addTopic(string topicName, string typeName){
	map<string, TypeSupport>::iterator iter = types_.find(typeName);
	if(iter == types_.end()){
		cout<<"addTopic Fail: no such type: "<<typeName<<endl;
		return false;
	}
	Topic* tmp = participant_->create_topic(topicName,(iter->second).get_type_name(), TOPIC_QOS_DEFAULT);
	if(tmp == nullptr){
		cout<<"addTopic Fail: create_topic: "<<topicName<<" fail"<<endl;
		return false;
	}
	topics_.insert(pair<string,Topic*>(topicName,tmp));
	return true;	
}
bool Bundle::addWriter(string name, string config, DataWriterListener* listener,BunWriter* bunwriter){
	map<string, Topic*>::iterator iter0 = topics_.find(bunwriter->topicName_);
	if(iter0 == topics_.end()){
		cout<<"addWriter Fail: no such topic: "<<bunwriter->topicName_<<endl;
		return false;
	}
	map<string, TypeSupport>::iterator iter1 = types_.find(bunwriter->typeName_);
	if(iter1 == types_.end()){
		cout<<"addWriter Fail: no such type: "<<bunwriter->typeName_<<endl;
		return false;
	}
	bunwriter->writer_ = publisher_->create_datawriter_with_profile(iter0->second, config, listener);
	if (bunwriter->writer_ == nullptr)
	{
		cout<<"addWriter Fail: writer init fail"<<endl;
		return false;
	}
	writers_.insert(pair<string, BunWriter*>(name,bunwriter));
	return true;
}
bool Bundle::addReader(string name, string topicName, string typeName, string config, DataReaderListener* listener){
	map<string, Topic*>::iterator iter0 = topics_.find(topicName);
	if(iter0 == topics_.end()){
		cout<<"addReader Fail: no such topic: "<<topicName<<endl;
		return false;
	}
	map<string, TypeSupport>::iterator iter1 = types_.find(typeName);
	if(iter1 == types_.end()){
		cout<<"addReader Fail: no such type: "<<typeName<<endl;
		return false;
	}
	BunReader* tmp = new BunReader(topicName,typeName);
	tmp->reader_ = subscriber_->create_datareader_with_profile(iter0->second, config, listener);
	if (tmp->reader_ == nullptr)
	{
		cout<<"addReader Fail: reader init fail"<<endl;
		return false;
	}
	readers_.insert(pair<string, BunReader*>(name,tmp));
	return true;
}
BunWriter* Bundle::getWriter(string name){
map<string, BunWriter*>::iterator iter = writers_.find(name);
if(iter == writers_.end()){
return nullptr;
}
return iter->second;
}
bool Bundle::send(string name,GeneralWriterListener* listener){
	map<string, BunWriter*>::iterator iter = writers_.find(name);
	if(iter == writers_.end()){
		cout<<"send Fail: no such writer: "<<name<<endl;
		return false;
	}
	if(listener->matched_ > 0){
		iter->second->send();
		cout<<"successful send"<<endl;
		return true;
	}else{
		cout<<"send Fail: no matched reader"<<endl;
		return false;
	}
}
