#include "Server.h"
#include <cstdlib>
#define TEST
class CustomDataWriterListener : public DataWriterListener
{
public:


std::atomic_int matched_;
    CustomDataWriterListener()
    : DataWriterListener()
    {
    }

    virtual ~CustomDataWriterListener()
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
