

#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "k_types.h"
#include "trainset_defs.h"

void packet_server();
void initPacketServer(packet_tracker_t* tracker);

void processTrainsetPacket(packet_t* pkt, packet_tracker_t* tracker);
bool processControlMessage(uint8_t* data, size_t size, pmbox_t src_box);

inline void flushPacketTable(uint8_t LastReceived);

inline bool sendPacket(packet_t* pkt, packet_tracker_t* tracker);
inline bool sendAck(packet_tracker_t* tracker);
inline void sendNack(packet_tracker_t* tracker);

#endif // PACKET_HANDLER_H
