

#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <stdint.h>
#include <stdbool.h>
#include "k_types.h"

#define PACKET_BOX          14
#define PACKET_MAX          8
#define PACKET_DATA_MAX     256

#define PACKET_META_SIZE    2

typedef struct PACKET_TYPES_ {DATA, ACK, NACK} packet_type_t;

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
    packet_entry_t  packet[PACKET_MAX];
    uint8_t         last_valid;
    uint8_t         Ns;
    uint8_t         Nr;
} packet_table_t;


void packet_server();
void initPacketServer(packet_table_t* table, packet_t* ack);



#endif // PACKET_HANDLER_H
