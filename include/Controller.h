#include "ConSerPubSubTypes.h"
#include "SerConPubSubTypes.h"

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
class ConSub
{
private:

    DomainParticipant* participant_;

    Subscriber* subscriber_;

    DataReader* reader_;

    Topic* topic_;

    TypeSupport type_;

    string mainPath_;
    
    public:

    ConSub();

    ~ConSub();
        
    //!Initialize the subscriber
    bool init();
    
    //!Run the Subscriber
    void run();	    
};
class ConPub
{
private:

    DomainParticipant* participant_;

    Publisher* publisher_;

    Topic* topic_;

    DataWriter* writer_;

    TypeSupport type_;

    string mainPath_;
   
    ConSer message_;
    public:

    ConPub();

    ~ConPub();
        
    //!Initialize the subscriber
    bool init();
    
    //!Run the Subscriber
    void run();	    
};
