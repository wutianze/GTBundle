#include "Server.h"
#include <cstdlib>
#define TEST
//------------------SerCliPub------------------------
class SerCliWriterListener : public DataWriterListener
{
public:
std::atomic_int matched_;
    SerCliWriterListener()
    : DataWriterListener()
    {
    }

    virtual ~SerCliWriterListener()
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
         DataWriter* /*writer*/,
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
}serclipub_listener_;


SerCliPub::SerCliPub()
: participant_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , writer_(nullptr)
        , type_(new SerCliPubSubType())
{
	cout<<"new SerCliPub"<<endl;
mainPath_ = string(getenv("GTPATH"));
}

SerCliPub::~SerCliPub()
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
bool SerCliPub::init(){
	cout<<"SerCliPub init"<<endl;
DomainParticipantFactory::get_instance()->load_XML_profiles_file(mainPath_+"/config/profiles.xml"); 
	participant_ = DomainParticipantFactory::get_instance()->create_participant_with_profile(0, "serclipub_participant");

        if (participant_ == nullptr)
        {
		cout<<"participant init fail"<<endl;
            return false;
        }

        // Register the Type
        type_.register_type(participant_);

        // Create the subscriptions Topic
        topic_ = participant_->create_topic("SerCli0", type_.get_type_name() , TOPIC_QOS_DEFAULT);

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
        writer_ = publisher_->create_datawriter_with_profile(topic_, "serclipub_datawriter", &serclipub_listener_);

        if (writer_ == nullptr)
        {

		cout<<"writer init fail"<<endl;
            return false;
        }

        return true;
}
void SerCliPub::run(){
#ifdef TEST
	cout<<"SerCliPub run"<<endl;
	uint32_t test_send = 5;
	uint64_t i = 0;
while(i<test_send){	
        while(serclipub_listener_.matched_ > 0){
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

//---------------CliSerSub-----------------
class CliSerReaderListener : public DataReaderListener
{
public:

    CliSerReaderListener()
    : DataReaderListener()
    {
    }

    virtual ~CliSerReaderListener()
    {
    }

    virtual void on_data_available(
            DataReader* reader)
    {
        (void)reader;
        std::cout << "Received new data message" << std::endl;
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
            DataReader* /*reader*/,
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
}clisersub_listener_;

CliSerSub::CliSerSub()
        : participant_(nullptr)
        , subscriber_(nullptr)
        , topic_(nullptr)
        , reader_(nullptr)
        , type_(new CliSerPubSubType())
    {
	    mainPath_ = string(getenv("GTPATH"));
    };
CliSerSub::~CliSerSub(){
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
bool CliSerSub::init(){
DomainParticipantFactory::get_instance()->load_XML_profiles_file(mainPath_+"/config/profiles.xml"); 
	participant_ = DomainParticipantFactory::get_instance()->create_participant_with_profile(0, "clisersub_participant");

        if (participant_ == nullptr)
        {
            return false;
        }

        // Register the Type
        type_.register_type(participant_);

        // Create the subscriptions Topic
        topic_ = participant_->create_topic("CliSer0", type_.get_type_name() , TOPIC_QOS_DEFAULT);

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
        reader_ = subscriber_->create_datareader_with_profile(topic_, "clisersub0_datareader", &clisersub_listener_);

        if (reader_ == nullptr)
        {
            return false;
        }

        return true;

}
void CliSerSub::run(){
while(true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
}
//------------------SerConPub------------------------
class SerConWriterListener : public DataWriterListener
{
public:
std::atomic_int matched_;
    SerConWriterListener()
    : DataWriterListener()
    {
    }

    virtual ~SerConWriterListener()
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
         DataWriter* /*writer*/,
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
}serconpub_listener_;


SerConPub::SerConPub()
: participant_(nullptr)
        , publisher_(nullptr)
        , topic_(nullptr)
        , writer_(nullptr)
        , type_(new SerConPubSubType())
{
	cout<<"new SerConPub"<<endl;
mainPath_ = string(getenv("GTPATH"));
}

SerConPub::~SerConPub()
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
bool SerConPub::init(){
	cout<<"SerConPub init"<<endl;
DomainParticipantFactory::get_instance()->load_XML_profiles_file(mainPath_+"/config/profiles.xml"); 
	participant_ = DomainParticipantFactory::get_instance()->create_participant_with_profile(0, "serconpub_participant");

        if (participant_ == nullptr)
        {
		cout<<"participant init fail"<<endl;
            return false;
        }

        // Register the Type
        type_.register_type(participant_);

        // Create the subscriptions Topic
        topic_ = participant_->create_topic("SerCon0", type_.get_type_name() , TOPIC_QOS_DEFAULT);

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
        writer_ = publisher_->create_datawriter_with_profile(topic_, "serconpub_datawriter", &serclipub_listener_);

        if (writer_ == nullptr)
        {

		cout<<"writer init fail"<<endl;
            return false;
        }

        return true;
}
void SerConPub::run(){
#ifdef TEST
	cout<<"SerConPub run"<<endl;
	uint32_t test_send = 5;
	uint64_t i = 0;
while(i<test_send){	
        while(serconpub_listener_.matched_ > 0){
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

//---------------ConSerSub-----------------
class ConSerReaderListener : public DataReaderListener
{
public:

    ConSerReaderListener()
    : DataReaderListener()
    {
    }

    virtual ~ConSerReaderListener()
    {
    }

    virtual void on_data_available(
            DataReader* reader)
    {
        (void)reader;
        std::cout << "Received new data message" << std::endl;
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
            DataReader* /*reader*/,
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
}consersub_listener_;

ConSerSub::ConSerSub()
        : participant_(nullptr)
        , subscriber_(nullptr)
        , topic_(nullptr)
        , reader_(nullptr)
        , type_(new ConSerPubSubType())
    {
	    mainPath_ = string(getenv("GTPATH"));
    };
ConSerSub::~ConSerSub(){
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
bool ConSerSub::init(){
DomainParticipantFactory::get_instance()->load_XML_profiles_file(mainPath_+"/config/profiles.xml"); 
	participant_ = DomainParticipantFactory::get_instance()->create_participant_with_profile(0, "consersub_participant");

        if (participant_ == nullptr)
        {
            return false;
        }

        // Register the Type
        type_.register_type(participant_);

        // Create the subscriptions Topic
        topic_ = participant_->create_topic("ConSer0", type_.get_type_name() , TOPIC_QOS_DEFAULT);

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
        reader_ = subscriber_->create_datareader_with_profile(topic_, "consersub0_datareader", &clisersub_listener_);

        if (reader_ == nullptr)
        {
            return false;
        }

        return true;

}
void ConSerSub::run(){
while(true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
}
