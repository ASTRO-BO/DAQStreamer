#include "CameraEventStreamer.h"
#include <packet/PacketBufferV.h>

#define DEBUG 1

namespace RTA
{

CameraEventStreamer::CameraEventStreamer(std::string config_filename, std::string input_filename, uint32 num_events_to_send)
 : Streamer("CameraEventStreamer"), _num_sent(0), _num_to_send(num_events_to_send),
   _config_filename(config_filename), _input_filename(input_filename), _buff(nullptr), _ps(nullptr) {
}

void CameraEventStreamer::thread_init()
{
	std::cout << "init" << std::endl;

	if(_buff) // this should not happen to be true
		delete _buff;
	if(_ps)
		delete _ps;

	_buff = new PacketLib::PacketBufferV(_config_filename, _input_filename);
	_buff->load();

	_ps = new PacketLib::PacketStream(_config_filename.c_str());
	std::cout << "end init" << std::endl;
}

void CameraEventStreamer::thread_loop()
{
	std::cout << "loop" << std::endl;
    PacketLib::ByteStreamPtr rawPacket = _buff->getNext();
    PacketLib::Packet *p = _ps->getPacket(rawPacket);

    CTADataModel::CameraEvent* event = _packetToCameraEventMessage(p);

    int messageSize = event->ByteSize();
#ifdef DEBUG
    std::cout << "Message size: " << messageSize << std::endl;
#endif

//	event->PrintDebugString();
    std::cout << _num_sent << std::endl;

    //Send one message (i.e. one cameraEvent)
    sendMessage(*event);

    _num_sent++;

    //If we've reached the target number of events, send END_OF_STREAM to stop the downstream pipeline
    if (_num_sent >= _num_to_send)
    {
        sendEOS();

        //This will make the current thread to join at the end of this function call
        _continue = false;
    }

    //send 10 events per second
    usleep(100000);
}

void CameraEventStreamer::thread_destroy()
{
	delete _buff;
	delete _ps;
}

CTADataModel::CameraEvent* CameraEventStreamer::_packetToCameraEventMessage(PacketLib::Packet* packet)
{
    PacketLib::DataFieldHeader* dfh = packet->getPacketDataFieldHeader();
    PacketLib::SourceDataField* sdf = packet->getPacketSourceDataField();
    const unsigned int telescopeID = sdf->getFieldValue_16ui("TelescopeID");
    // FIXME dateMJD should be something like:
    // unsigned int LTtime = dfh->getFieldValue_64ui("LTtime");
    // handle the 64 bits
    const unsigned int dateMJD = 1;
    const CTADataModel::EventType eventType = CTADataModel::EventType::PHYSICAL;
    const unsigned int eventNumber = sdf->getFieldValue_32ui("eventNumber");
    const unsigned int arrayEvtNum = dfh->getFieldValue_32ui("ArrayID");
    PacketLib::byte* data = packet->getData()->getStream();
	PacketLib::dword size = packet->getData()->size();
	
    const unsigned int num_samples = sdf->getFieldValue_32ui("Number of samples");

#ifdef DEBUG
    std::cout << "telescopeid   time   eventtype   eventNumber   arrayEvtNum   num_samples  " << std::endl;
    std::cout << telescopeID << "   " << dateMJD << "   " << eventType << "   " << eventNumber;
    std::cout << "   " << arrayEvtNum << "   " << num_samples << std::endl;
#endif

    // Fill a new event
    CTADataModel::CameraEvent* event = new CTADataModel::CameraEvent();
    event->set_telescopeid(telescopeID);
    event->set_datemjd(dateMJD);
    event->set_eventtype(eventType);
    event->set_eventnumber(eventNumber);
    event->set_arrayevtnum(arrayEvtNum);

    CTADataModel::PixelsChannel* higain = event->mutable_higain();
    CTADataModel::WaveFormData* waveforms = higain->mutable_waveforms();
    CTADataModel::CTAArray* samples = waveforms->mutable_samples(); 
    samples->set_data((const char*)data, size);
    waveforms->set_num_samples(num_samples);

    return event;
}

};
