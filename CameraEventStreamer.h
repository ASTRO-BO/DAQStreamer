/*
 *  CameraEventStreamer
 *
 *  Created on Nov 26, 2014
 *  Author: zoli
 */
#ifndef CAMERA_EVENT_STREAMER_H_
#define CAMERA_EVENT_STREAMER_H_

#include "Streamer.hpp"
#include "ThreadedObject.h"
#include "L0.pb.h"
#include <packet/PacketStream.h>
#include <packet/PacketBufferV.h>

namespace RTA
{

/**
 *  @class CameraEventStreamer
 *  @brief brief.
 *
 *  Long.
 *
 */
class CameraEventStreamer : public CTATools::Core::Streamer,
                            public CTATools::Core::ThreadedObject
{

public:

    /**
     *   @brief default constructor
     *
     *   @param num_events_to_send the number of dummy events to generate
     */
    CameraEventStreamer(std::string config_filename, std::string input_filename, uint32 num_events_to_send);

protected:

    /**
     *    @brief initialization before serving the events
     *
     *    This is an overload of the ThreadedObject
     */
    void thread_init();

    /**
     *  @brief main method called in loop. Sends one camera event at a time and sleeps for 1/10 of a second
     *
     *  This is an overload of the ThreadedObject class
     */
    void thread_loop();

    /**
     *  @brief destroy of the thread.
     *
     *  Long.
     */
    void thread_destroy();

private:
    uint32                     _num_sent;         ///< The number of events sent so far
    uint32                     _num_to_send;      ///< The number of events to send

	std::string                _config_filename;  ///< The config filename (.xml)
	std::string                _input_filename;   ///< The raw packet filename (.raw)
	PacketLib::PacketBufferV*  _buff;             ///< The circular buffer of packets that will be sent
	PacketLib::PacketStream*   _ps;               ///< The packet stream is used to get a packet from a bytestream

	CTADataModel::CameraEvent* _packetToCameraEventMessage(PacketLib::Packet* packet);
};

};
#endif
