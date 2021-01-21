#ifndef LISTENER_H
#define LISTENER_H
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
#include "AccessServer.h"
using namespace eprosima::fastdds::dds;
using namespace eprosima::fastdds::rtps;
using namespace std;
class AccessServer;
class CliReaderListener : public DataReaderListener
{

public:
    SerCli message_;
    atomic_int samples_;
    AccessServer* as_;

    CliReaderListener()
    : DataReaderListener()
    {
	    samples_=0;
    };

    virtual ~CliReaderListener()
    {
	    cout<<"delete CliReaderListener"<<endl;
    };

    void setSocketServer(AccessServer* as){
    as_ = as;
    }
     void on_data_available(
            DataReader* reader);
    
     void on_subscription_matched(
            DataReader* reader,
            const SubscriptionMatchedStatus& info);
    

     void on_requested_deadline_missed(
            DataReader* reader,
            const eprosima::fastrtps::RequestedDeadlineMissedStatus& info);
    

     void on_liveliness_changed(
            DataReader* reader,
            const eprosima::fastrtps::LivelinessChangedStatus& info);
    

     void on_sample_rejected(
            DataReader* reader,
            const eprosima::fastrtps::SampleRejectedStatus& info);
    

     void on_requested_incompatible_qos(
            DataReader* /*reader*/,
            const RequestedIncompatibleQosStatus& info);
    

     void on_sample_lost(
            DataReader* reader,
            const SampleLostStatus& info);
    };
class CliSerReaderListener : public DataReaderListener
{

public:
    CliSer message_;
    atomic_int samples_;
    AccessServer* as_;

    CliSerReaderListener()
    : DataReaderListener()
    {
	    cout<<"CliSerReaderListener created"<<endl;
	    samples_=0;
    };

    virtual ~CliSerReaderListener()
    {
	    cout<<"delete CliSerReaderListener"<<endl;
    };
void setSocketServer(AccessServer* as){
    as_ = as;
    }
     void on_data_available(
            DataReader* reader);
    
     void on_subscription_matched(
            DataReader* reader,
            const SubscriptionMatchedStatus& info);
    
     void on_requested_deadline_missed(
            DataReader* reader,
            const eprosima::fastrtps::RequestedDeadlineMissedStatus& info);
    
     void on_liveliness_changed(
            DataReader* reader,
            const eprosima::fastrtps::LivelinessChangedStatus& info);
    
     void on_sample_rejected(
            DataReader* reader,
            const eprosima::fastrtps::SampleRejectedStatus& info);
    
     void on_requested_incompatible_qos(
            DataReader* ,
            const RequestedIncompatibleQosStatus& info);
    
     void on_sample_lost(
            DataReader* reader,
            const SampleLostStatus& info);
    };
class GeneralWriterListener : public DataWriterListener
{
public:
std::atomic_int matched_;
    GeneralWriterListener()
    : DataWriterListener()
    {
	    matched_=0;
    };

    virtual ~GeneralWriterListener()
    {
	    cout<<"delete GeneralWriterLister"<<endl;
    };

     void on_publication_matched(
            DataWriter* writer,
            const PublicationMatchedStatus& info);
    
     void on_offered_deadline_missed(
             DataWriter* writer,
             const OfferedDeadlineMissedStatus& status);
         void on_offered_incompatible_qos(
         DataWriter* /*writer*/,
         const OfferedIncompatibleQosStatus& status);
    
     void on_liveliness_lost(
         DataWriter* writer,
         const LivelinessLostStatus& status);
    };

#endif
