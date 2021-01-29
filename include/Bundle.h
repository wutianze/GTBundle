#ifndef BUNDLE_H
#define BUNDLE_H
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
#include "LogUpdate.h"
using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;
using namespace std;
class GeneralWriterListener;
class BunReader{
	public:
    DataReader* reader_;

    DataReaderListener* reader_listener_;
    string topicName_;

    string typeName_;


BunReader(string tn, string tyn):reader_(nullptr),topicName_(tn),typeName_(tyn){};
virtual ~BunReader(){
logUpdate("delete BunReader",Nor);
delete reader_listener_;
}
};
class BunWriter{
	public:
    DataWriter* writer_;
    DataWriterListener* writer_listener_;
    string topicName_;

    string typeName_;
    virtual bool send();
    virtual ~BunWriter(){
    logUpdate("delete BunWriter",Nor);
    delete writer_listener_;
    };
    BunWriter(string tn, string tyn):writer_(nullptr),topicName_(tn),typeName_(tyn){};
};
class CliWriter:public BunWriter{
	public:
		CliWriter(string tn, string tyn):BunWriter(tn,tyn){};
	CliSer message_;
virtual bool send();
virtual ~CliWriter(){
    logUpdate("delete CliWriter",Nor);
};
};
class SerCliWriter:public BunWriter{
	public:
		SerCliWriter(string tn, string tyn):BunWriter(tn,tyn){};
	SerCli message_;
virtual bool send();
virtual ~SerCliWriter(){
    logUpdate("delete SerCliWriter",Nor);
};
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
    bool addWriter(string name, string config, DataWriterListener* listener,BunWriter* bunwriter);
    bool addReader(string name, string topicName,string typeName, string config, DataReaderListener* listener);
    BunWriter* getWriter(string name);
    //bool send(string name,GeneralWriterListener*listener);
};
#endif
