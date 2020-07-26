#include "SerCliPubSubTypes.h"
#include "CliSerPubSubTypes.h"
#include "SerConPubSubTypes.h"
#include "ConSerPubSubTypes.h"

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
using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;
using namespace std;
class SerCliPub
{
private:

    DomainParticipant* participant_;

    Publisher* publisher_;

    Topic* topic_;

    DataWriter* writer_;

    TypeSupport type_;

    string mainPath_;
   
    SerCli message_;
    public:

    SerCliPub();

    ~SerCliPub();
        
    //!Initialize the subscriber
    bool init();
    
    //!Run the Subscriber
    void run();	    

    bool send();
};
class CliSerSub
{
private:
    
    DomainParticipant* participant_;

    Subscriber* subscriber_;

    DataReader* reader_;

    Topic* topic_;

    TypeSupport type_;

    string mainPath_;
    
    public:

    CliSerSub();

    ~CliSerSub();
        
    //!Initialize the subscriber
    bool init();
    
    //!Run the Subscriber
    void run();	    

};
class SerConPub
{
private:

    DomainParticipant* participant_;

    Publisher* publisher_;

    Topic* topic_;

    DataWriter* writer_;

    TypeSupport type_;

    string mainPath_;
   
    SerCon message_;
    public:

    SerConPub();

    ~SerConPub();
        
    //!Initialize the subscriber
    bool init();
    
    //!Run the Subscriber
    void run();	    
};
class ConSerSub
{
private:
    
    DomainParticipant* participant_;

    Subscriber* subscriber_;

    DataReader* reader_;

    Topic* topic_;

    TypeSupport type_;

    string mainPath_;
    
    public:

    ConSerSub();

    ~ConSerSub();
        
    //!Initialize the subscriber
    bool init();
    
    //!Run the Subscriber
    void run();	    

};
