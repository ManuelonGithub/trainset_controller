

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
    uint8_t         nr : 3;
    uint8_t         ns : 3;
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
    packet_entry_t  entry[PACKET_MAX];
    uint8_t         valid_ptr;
    uint8_t         free_ptr;
    bool            full;
} packet_table_t;

#define PKT_TRACK_MOV(ptr) (ptr = (ptr + 1) & (PACKET_MAX-1))

typedef struct packet_tracker_ {
    uint8_t         Ns;
    uint8_t         Nr;
} packet_tracker_t;

void packet_server();
void initPacketServer();

void processTrainsetPacket(packet_t* pkt);
bool processControlMessage(uint8_t* data, size_t size, pmbox_t src_box);

inline void flushPacketTable(uint8_t LastReceived);

inline void SendAck();
inline void SendNack();

inline uint8_t pktTableSpace(packet_table_t* table);
inline void pktTableEnqueue(packet_table_t* table);
inline void pktTableDequeue(packet_table_t* table);

#endif // PACKET_HANDLER_H
