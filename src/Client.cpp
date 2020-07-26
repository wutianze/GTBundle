#include "Client.h"
#include <cstdlib>
#define TEST
//------------------CliSub------------------------
/*class CliSubReaderListener : public DataReaderListener
{

public:
    SerCli message_;
    atomic_int samples_;
    CliSubReaderListener()
    : DataReaderListener()
    {
	    samples_=0;
    }

    virtual ~CliSubReaderListener()
    {
    }

    virtual void on_data_available(
            DataReader* reader)
    {
        SampleInfo info;
            if (reader->take_next_sample(&message_, &info) == ReturnCode_t::RETCODE_OK)
            {
		    //auto rec_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch()).count();
		    if (info.instance_state == ALIVE)
                {
                    samples_++;
                    std::cout << " with seq: " << message_.seq()//<<" with key: "<<message_.kk()
                                << " samples:" <<samples_<<std::endl;
		
		}
            }
    }

    virtual void on_subscription_matched(
            DataReader* reader,
            const SubscriptionMatchedStatus& info)
    {
        (void)reader;
        if (info.current_count_change == 1)
        {
            std::cout << "Matched a remote DataWriter" << std::endl;
        }
        else if (info.current_count_change == -1)
        {
            std::cout << "Unmatched a remote DataWriter" << std::endl;
        }
    }

    virtual void on_requested_deadline_missed(
            DataReader* reader,
            const eprosima::fastrtps::RequestedDeadlineMissedStatus& info)
    {
        (void)reader, (void)info;
        std::cout << "Some data was not received on time" << std::endl;
    }

    virtual void on_liveliness_changed(
            DataReader* reader,
            const eprosima::fastrtps::LivelinessChangedStatus& info)
    {
        (void)reader;
        if (info.alive_count_change == 1)
        {
            std::cout << "A matched DataWriter has become active" << std::endl;
        }
        else if (info.not_alive_count_change == 1)
        {
            std::cout << "A matched DataWriter has become inactive" << std::endl;
        }
    }

    virtual void on_sample_rejected(
            DataReader* reader,
            const eprosima::fastrtps::SampleRejectedStatus& info)
    {
        (void)reader, (void)info;
        std::cout << "A received data sample was rejected" << std::endl;
    }

    virtual void on_requested_incompatible_qos(
            DataReader*,
            const RequestedIncompatibleQosStatus& info)
    {
        std::cout << "Found a remote Topic with incompatible QoS (QoS ID: " << info.last_policy_id <<
                ")" <<std::endl;
    }

    virtual void on_sample_lost(
            DataReader* reader,
            const SampleLostStatus& info)
    {
        (void)reader, (void)info;
        std::cout << "A data sample was lost and will not be received" << std::endl;
    }
}clisub_listener_;

CliSub::CliSub()
        : participant_(nullptr)
        , subscriber_(nullptr)
        , topic_(nullptr)
        , reader_(nullptr)
        , type_(new SerCliPubSubType())
    {
	    mainPath_ = string(getenv("GTPATH"));
    };
CliSub::~CliSub(){
	if (reader_ != nullptr)
        {
            subscriber_->delete_datareader(reader_);
        }
        if (topic_ != nullptr)
        {
            participant_->delete_topic(topic_);
        }
        if (subscriber_ != nullptr)
        {
            participant_->delete_subscriber(subscriber_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);

}
bool CliSub::init(){
DomainParticipantFactory::get_instance()->load_XML_profiles_file(mainPath_+"/config/profiles.xml"); 
	participant_ = DomainParticipantFactory::get_instance()->create_participant_with_profile(0, "clisub_participant");

        if (participant_ == nullptr)
        {
            return false;
        }

        // Register the Type
        type_.register_type(participant_);

        // Create the subscriptions Topic
        topic_ = participant_->create_topic("SerCli0", type_.get_type_name() , TOPIC_QOS_DEFAULT);

        if (topic_ == nullptr)
        {
            return false;
        }

        // Create the Subscriber
        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);

        if (subscriber_ == nullptr)
        {
            return false;
        }

        // Create the DataReader
        reader_ = subscriber_->create_datareader_with_profile(topic_, "clisub0_datareader", &clisub_listener_);
	DataReaderQos qos = reader_->get_qos();
	qos.ownership().kind = EXCLUSIVE_OWNERSHIP_QOS;
	reader_->set_qos(qos);
        if (reader_ == nullptr)
        {
            return false;
        }

        return true;

}
//---------------CliPub-----------------
class CliWriterListener : public DataWriterListener
{
public:
std::atomic_int matched_;
    CliWriterListener()
    : DataWriterListener()
    {
    }

    virtual ~CliWriterListener()
    {
    }

    virtual void on_publication_matched(
            DataWriter* writer,
            const PublicationMatchedStatus& info)
    {
        (void)writer;
	

        if (info.current_count_change == 1)
        {
            matched_ = info.total_count;
            std::cout << "Matched a remote Subscriber for one of our Topics" << std::endl;
        }
        else if (info.current_count_change == -1)
        {
            matched_ = info.total_count;
            std::cout << "Unmatched a remote Subscriber" << std::endl;
        }
    }

    virtual void on_offered_deadline_missed(
             DataWriter* writer,
             const OfferedDeadlineMissedStatus& status)
    {
         (void)writer, (void)status;
         std::cout << "Some data could not be delivered on time" << std::endl;
    }

    virtual void on_offered_incompatible_qos(
         DataWriter* ,
         const OfferedIncompatibleQosStatus& status)
    {
        std::cout << "Found a remote Topic with incompatible QoS (QoS ID: " << status.last_policy_id <<
                ")" <<std::endl;
    }

    virtual void on_liveliness_lost(
         DataWriter* writer,
         const LivelinessLostStatus& status)
    {
        (void)writer, (void)status;
        std::cout << "Liveliness lost. Matched Subscribers will consider us offline" << std::endl;
    }
}clipub_listener_;


CliPub::CliPub()
: participant_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , writer_(nullptr)
        , type_(new CliSerPubSubType())
{
	cout<<"new CliPub"<<endl;
mainPath_ = string(getenv("GTPATH"));
}

CliPub::~CliPub()
    {
        if (writer_ != nullptr)
        {
            publisher_->delete_datawriter(writer_);
        }
        if (publisher_ != nullptr)
        {
            participant_->delete_publisher(publisher_);
        }
        if (topic_ != nullptr)
        {
            participant_->delete_topic(topic_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }
bool CliPub::init(){
	cout<<"CliPub init"<<endl;
DomainParticipantFactory::get_instance()->load_XML_profiles_file(mainPath_+"/config/profiles.xml"); 
	participant_ = DomainParticipantFactory::get_instance()->create_participant_with_profile(0, "clisub_participant");

        if (participant_ == nullptr)
        {
		cout<<"participant init fail"<<endl;
            return false;
        }

        // Register the Type
        type_.register_type(participant_);

        // Create the subscriptions Topic
        topic_ = participant_->create_topic("CliSer0", type_.get_type_name() , TOPIC_QOS_DEFAULT);

        if (topic_ == nullptr)
        {
		cout<<"topic init fail"<<endl;
            return false;
        }

        // Create the Subscriber
        publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);

        if (publisher_ == nullptr)
        {
		cout<<"publisher init fail"<<endl;
            return false;
        }

        // Create the DataReader
        writer_ = publisher_->create_datawriter_with_profile(topic_, "clipub0_datawriter", &clipub_listener_);

        if (writer_ == nullptr)
        {

		cout<<"writer init fail"<<endl;
            return false;
        }

        return true;
}
void CliPub::run(){
#ifdef TEST
	cout<<"CliPub run"<<endl;
	uint32_t test_send = 5;
	uint64_t i = 0;
while(i<test_send){	
        while(clipub_listener_.matched_ > 0){
		message_.seq(i);
		i++;
		if(i>=test_send)break;
writer_->write(&message_);
		cout<<"publish one"<<endl;
}
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}
#endif
}

bool CliPub::send(){
if(clipub_listener_.matched_>0){
	writer_->write(&message_);
	return true;
}
return false;
}*/
//---------------------
Client::Client(string config)
: participant_(nullptr)
{
	cout<<"new Client"<<endl;
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
    Client::~Client(){
	    for(map<string, CliReader*>::iterator iter = readers_.begin();iter!= readers_.end();iter++){
	    	if(iter->second->reader_ != nullptr)subscriber_->delete_datareader(iter->second->reader_);
	    }
	    	if(subscriber_ != nullptr)participant_->delete_subscriber(subscriber_);
	for(map<string, CliWriter*>::iterator iter = writers_.begin();iter!=writers_.end();iter++){
	    	if(iter->second->writer_ != nullptr)publisher_->delete_datawriter(iter->second->writer_);
	    }
	    	if(publisher_ != nullptr)participant_->delete_publisher(publisher_);
for(map<string, Topic*>::iterator iter = topics_.begin();iter!=topics_.end();iter++){
	    	if(iter->second != nullptr)participant_->delete_topic(iter->second);
	    }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
}

bool Client::addTopic(string topicName, string typeName){
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
bool Client::addWriter(string name, string topicName, string typeName, string config, DataWriterListener* listener){
map<string, Topic*>::iterator iter0 = topics_.find(topicName);
if(iter0 == topics_.end()){
cout<<"addWriter Fail: no such topic: "<<topicName<<endl;
return false;
}
map<string, TypeSupport>::iterator iter1 = types_.find(typeName);
if(iter1 == types_.end()){
cout<<"addWriter Fail: no such type: "<<typeName<<endl;
return false;
}
CliWriter* tmp = new CliWriter(topicName,typeName);
tmp->writer_ = publisher_->create_datawriter_with_profile(iter0->second, config, listener);
if (tmp->writer_ == nullptr)
        {
	cout<<"addWriter Fail: writer init fail"<<endl;
            return false;
        }
writers_.insert(pair<string, CliWriter*>(name,tmp));
return true;
}
bool Client::addReader(string name, string topicName, string typeName, string config, DataReaderListener* listener){
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
CliReader* tmp = new CliReader(topicName,typeName);
tmp->reader_ = subscriber_->create_datareader_with_profile(iter0->second, config, listener);
if (tmp->reader_ == nullptr)
        {
	cout<<"addReader Fail: reader init fail"<<endl;
            return false;
        }
readers_.insert(pair<string, CliReader*>(name,tmp));
return true;
}
bool Client::send(string name,CliWriterListener* listener, int seq){
map<string, CliWriter*>::iterator iter = writers_.find(name);
if(iter == writers_.end()){
cout<<"send Fail: no such writer: "<<name<<endl;
return false;
}
	if(listener->matched_ > 0){
		iter->second->message_.seq(seq);
		iter->second->writer_->write(&(iter->second->message_));
		return true;
	}else{
	cout<<"send Fail: no matched reader"<<endl;
return false;
	}
}
