

#ifndef TRAINSET_DEFINITIONS_H
#define TRAINSET_DEFINITIONS_H

#include <stdint.h>

#define PACKET_BOX          14
#define TRACK_BOX           13

#define PACKET_NAME "Packet Server"
#define TRACK_NAME  "Track Manager"
#define TRAIN_NAME  "Train Manager"

typedef enum TRAIN_DIRECTIONS {CW, CCW} direction_t;

typedef struct trainset_msg_ {
    uint8_t code;
    uint8_t arg1;
    uint8_t arg2;
} trainset_msg_t;

typedef union train_ctrl_ {
    uint8_t arg;
    struct {
        uint8_t     mag : 4;
        uint8_t     res : 3;
        direction_t dir : 1;
    };
} train_ctrl_t;

#endif // TRAINSET_DEFINITIONS_H
