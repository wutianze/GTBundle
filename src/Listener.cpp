#include"Listener.h"   
     void CliReaderListener::on_data_available(
            DataReader* reader)
    {
        SampleInfo info;
            while(reader->take_next_sample(&message_, &info) == ReturnCode_t::RETCODE_OK)
            {
		    //auto rec_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch()).count();
		    if (info.instance_state == ALIVE)
                {
                    samples_++;
                    //logUpdate("listener receive from dds server with seq: " << message_.seq()<< " samples:" <<samples_<<", content: "<<message_.com(),Nor);
		    

		    if(!as_->Send(message_.com())){
		    //logUpdate("===> socket client fail",Err);
		    }// com is the ControllerJSON
		    
		    
		}
            }
    }

     void CliReaderListener::on_subscription_matched(
            DataReader* reader,
            const SubscriptionMatchedStatus& info)
    {
        (void)reader;
        if (info.current_count_change == 1)
        {
            logUpdate("Matched a remote DataWriter" ,Nor);
	    //as_->Send("{\"msgType\":\"bundle_publisher\",\"action\":\"matched\"}");
        }
        else if (info.current_count_change == -1)
        {
            logUpdate("Unmatched a remote DataWriter" ,Nor);
	    //as_->Send("{\"msgType\":\"bundle_publisher\",\"action\":\"unmatched\"}");
        }
    }

     void CliReaderListener::on_requested_deadline_missed(
            DataReader* reader,
            const eprosima::fastrtps::RequestedDeadlineMissedStatus& info)
    {
        (void)reader, (void)info;
        logUpdate("Some data was not received on time" ,Nor);
    }

     void CliReaderListener::on_liveliness_changed(
            DataReader* reader,
            const eprosima::fastrtps::LivelinessChangedStatus& info)
    {
        (void)reader;
        if (info.alive_count_change == 1)
        {
            logUpdate("A matched DataWriter has become active" ,Nor);
        }
        else if (info.not_alive_count_change == 1)
        {
            logUpdate("A matched DataWriter has become inactive" ,Nor);
        }
    }

     void CliReaderListener::on_sample_rejected(
            DataReader* reader,
            const eprosima::fastrtps::SampleRejectedStatus& info)
    {
        (void)reader, (void)info;
        logUpdate("A received data sample was rejected" ,Nor);
    }

     void CliReaderListener::on_requested_incompatible_qos(
            DataReader* /*reader*/,
            const RequestedIncompatibleQosStatus& info)
    {
        logUpdate("Found a remote Topic with incompatible QoS (QoS ID: " +to_string(info.last_policy_id)+")",Nor);
    }

     void CliReaderListener::on_sample_lost(
            DataReader* reader,
            const SampleLostStatus& info)
    {
        (void)reader, (void)info;
        logUpdate("A data sample was lost and will not be received" ,War);
    }

     void CliSerReaderListener::on_data_available(
            DataReader* reader)
    {
        SampleInfo info;
            while(reader->take_next_sample(&message_, &info) == ReturnCode_t::RETCODE_OK)
            {
		    //auto rec_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch()).count();
		    if (info.instance_state == ALIVE)
                {
                    samples_++;
		    if(!as_->Send(message_.com())){
		    //logUpdate("===> socket client fail",Err);
		    }// com is the GeneratorJSON 
		    
		    Document d;
		    d.Parse(message_.com().c_str());
		    //elasticsearch_cli_.addDoc(d["id"].GetString(),d["log"]);
		    elasticsearch_cli_.addDocWithTimestamp(d["id"].GetString(),d);
		    //int cpu_usage = d["log"]["cpu_usage"].GetInt();
		    //int cpu_allocated = d["log"]["cpu_allocated"].GetInt();
		    //cout<<"cpu usage"<<cpu_usage<<endl;

		    		}
            }
    }

     void CliSerReaderListener::on_subscription_matched(
            DataReader* reader,
            const SubscriptionMatchedStatus& info)
    {
        (void)reader;
        if (info.current_count_change == 1)
        {
            logUpdate("Matched a remote DataWriter" ,Nor);
	    //as_->Send("{\"msgType\":\"bundle_publisher\",\"action\":\"matched\"}");
        }
        else if (info.current_count_change == -1)
        {
            logUpdate("Unmatched a remote DataWriter" ,Nor);
	    //as_->Send("{\"msgType\":\"bundle_publisher\",\"action\":\"unmatched\"}");
        }
    }

     void CliSerReaderListener::on_requested_deadline_missed(
            DataReader* reader,
            const eprosima::fastrtps::RequestedDeadlineMissedStatus& info)
    {
        (void)reader, (void)info;
        logUpdate("Some data was not received on time" ,Nor);
    }

     void CliSerReaderListener::on_liveliness_changed(
            DataReader* reader,
            const eprosima::fastrtps::LivelinessChangedStatus& info)
    {
        (void)reader;
        if (info.alive_count_change == 1)
        {
            logUpdate("A matched DataWriter has become active" ,Nor);
        }
        else if (info.not_alive_count_change == 1)
        {
            logUpdate("A matched DataWriter has become inactive" ,Nor);
        }
    }

     void CliSerReaderListener::on_sample_rejected(
            DataReader* reader,
            const eprosima::fastrtps::SampleRejectedStatus& info)
    {
        (void)reader, (void)info;
        logUpdate("A received data sample was rejected" ,Nor);
    }

     void CliSerReaderListener::on_requested_incompatible_qos(
            DataReader*,
            const RequestedIncompatibleQosStatus& info)
    {
        logUpdate("Found a remote Topic with incompatible QoS (QoS ID: "+to_string(info.last_policy_id)+")",Nor);
    }

     void CliSerReaderListener::on_sample_lost(
            DataReader* reader,
            const SampleLostStatus& info)
    {
        (void)reader, (void)info;
        logUpdate("A data sample was lost and will not be received" ,Nor);
    }
     void GeneralWriterListener::on_publication_matched(
            DataWriter* writer,
            const PublicationMatchedStatus& info)
    {
        (void)writer;
	

        if (info.current_count_change == 1)
        {
            matched_ = info.total_count;
            logUpdate("Matched a remote Subscriber for one of our Topics" ,Nor);
	    //as_->Send("{\"msgType\":\"bundle_subscriber\",\"action\":\"matched\"}");
        }
        else if (info.current_count_change == -1)
        {
            matched_ = info.total_count;
            logUpdate("Unmatched a remote Subscriber" ,Nor);
	    //as_->Send("{\"msgType\":\"bundle_subscriber\",\"action\":\"unmatched\"}");
        }
    }

     void GeneralWriterListener::on_offered_deadline_missed(
             DataWriter* writer,
             const OfferedDeadlineMissedStatus& status)
    {
         (void)writer, (void)status;
         logUpdate("Some data could not be delivered on time" ,War);
    }

     void GeneralWriterListener::on_offered_incompatible_qos(
         DataWriter* /*writer*/,
         const OfferedIncompatibleQosStatus& status)
    {
        logUpdate("Found a remote Topic with incompatible QoS (QoS ID: " +to_string(status.last_policy_id)+")" ,War);
    }

     void GeneralWriterListener::on_liveliness_lost(
         DataWriter* writer,
         const LivelinessLostStatus& status)
    {
        (void)writer, (void)status;
        logUpdate("Liveliness lost. Matched Subscribers will consider us offline" ,War);
    }
