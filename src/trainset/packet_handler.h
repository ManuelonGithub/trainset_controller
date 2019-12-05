

#ifndef PACKET_HANDLER_H
#define PACKET_HANDLER_H

#include <stdint.h>
#include <stdbool.h>
#include "k_types.h"

#define PACKET_BOX  14

typedef struct PACKET_TYPES_ {DATA, ACK, NACK} packet_type_t;

typedef struct packet_ctrl_ {
    uint8_t         nr : 3;
    uint8_t         ns : 3;
    packet_type_t type : 2;
} packet_ctrl_t;

typedef struct packet_ {
    packet_ctrl_t   ctrl;
    uint8_t         length;
    uint8_t*        data;
} packet_t;

typedef struct packet_entry_ {
    bool        valid;
    pmbox_t     src_box;
    packet_t    packet;
};

typedef struct packet_entry

void packetHandletInit();



#endif // PACKET_HANDLER_H
