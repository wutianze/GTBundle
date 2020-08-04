#ifndef CLIENT_H
#define CLIENT_H
#include "CliSerPubSubTypes.h"
#include "SerCliPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/rtps/transport/TCPv4TransportDescriptor.h>
#include <fastrtps/utils/IPLocator.h>
#include <fastrtps/qos/QosPolicies.h>
#include <fstream>
#include <map>

#include "Listener.h"
using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;
using namespace std;
/*class CliSub
{
private:

    DomainParticipant* participant_;

    Subscriber* subscriber_;

    DataReader* reader_;

    Topic* topic_;

    TypeSupport type_;

    string mainPath_;
    
    public:

    CliSub();

    ~CliSub();
        
    //!Initialize the subscriber
    bool init();
    
};
class CliPub
{
private:

    DomainParticipant* participant_;

    Publisher* publisher_;

    Topic* topic_;

    DataWriter* writer_;

    TypeSupport type_;

    string mainPath_;
   
    CliSer message_;
    public:

    CliPub();

    ~CliPub();
        
    //!Initialize the subscriber
    bool init();
    
    //!Run the Publisher for test
    void run();	   

    bool send();
};*/
class BunReader{
	public:
    DataReader* reader_;

    string topicName_;

    string typeName_;


BunReader(string tn, string tyn):reader_(nullptr),topicName_(tn),typeName_(tyn){};
};
class BunWriter{
	public:
    DataWriter* writer_;

    string topicName_;

    string typeName_;

    CliSer message_;
    virtual send() = 0;
    BunWriter(string tn, string tyn):writer_(nullptr),topicName_(tn),typeName_(tyn){};
};

class Bundle
{
private:

    DomainParticipant* participant_;
    

    Publisher* publisher_;
    Subscriber* subscriber_;
    
    map<string, BunReader*> readers_;

    map<string, BunWriter*> writers_;
    
    map<string, Topic*> topics_;

    //TypeSupport is shared_ptr
    map<string, TypeSupport> types_;

    string mainPath_;

    public:

    Bundle(string config);

    ~Bundle();
        
    bool addTopic(string topicName,string typeName);
    bool addWriter(string name, string topicName,string typeName, string config, DataWriterListener* listener);
    bool addReader(string name, string topicName,string typeName, string config, DataReaderListener* listener);
    
    bool send(string name,CliWriterListener*listener, int seq);
};
#endif
