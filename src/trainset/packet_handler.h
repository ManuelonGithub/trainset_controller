

#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "k_types.h"

#define PACKET_MAX          8
#define PACKET_DATA_MAX     256

#define PACKET_META_SIZE    2

typedef enum PACKET_TYPES_ {DATA, ACK, NACK} packet_type_t;

typedef struct packet_ctrl_ {
    uint8_t         Nr : 3;
    uint8_t         Ns : 3;
    packet_type_t type : 2;
} packet_ctrl_t;

typedef struct packet_ {
    packet_ctrl_t   ctrl;
    uint8_t         length;
    uint8_t         data[PACKET_DATA_MAX];
} packet_t;

typedef struct packet_entry_ {
    bool        valid;
    pmbox_t     src_box;
    packet_t    packet;
} packet_entry_t;

typedef struct packet_table_ {
    packet_t    packet[PACKET_MAX];
    pmbox_t     packetSrc[PACKET_MAX];
    uint8_t     valid;
    uint8_t     free;
    bool        full;
} packet_table_t;

#define PKT_MOV(ptr) (ptr = (ptr + 1) & (PACKET_MAX-1))

typedef struct packet_tracker_ {
    uint8_t         Ns;
    uint8_t         Nr;
    bool            pend_ack;
} packet_tracker_t;

void packet_server();
void initPacketServer(packet_tracker_t* tracker);

void processTrainsetPacket(packet_t* pkt, packet_tracker_t* tracker);
bool processControlMessage(uint8_t* data, size_t size, pmbox_t src_box);

inline void flushPacketTable(uint8_t LastReceived);

inline bool sendPacket(packet_t* pkt, packet_tracker_t* tracker);
inline bool sendAck(packet_tracker_t* tracker);
inline void sendNack(packet_tracker_t* tracker);

#endif // PACKET_HANDLER_H
