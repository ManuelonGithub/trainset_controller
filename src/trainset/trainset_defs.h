

#ifndef TRAINSET_DEFINITIONS_H
#define TRAINSET_DEFINITIONS_H

#include <stdint.h>

#define PACKET_BOX          14
#define TRACK_BOX           13

#define PACKET_NAME "Packet Server"
#define TRACK_NAME  "Track Manager"
#define TRAIN_NAME  "Train Manager"

typedef enum TRAIN_DIRECTIONS {CW, CCW} direction_t;
typedef enum SWITCH_POSITIONS {DIVERTED, STRAIGHT} switch_pos_t;

typedef enum SENSOR_CODES {
    SENSOR_TRIGGERED    = 0xA0,
    SENSOR_TRIG_ACK     = 0xA2,
    SENSOR_RESET        = 0xA8,
    SENSOR_RST_ACK      = 0xAA
} sensor_code_t;

typedef enum TRAIN_CODES {
    TRAIN_MOVE    = 0xC0,
    TRAIN_ACK     = 0xC2
} train_code_t;

typedef enum SWITCH_CODES {
    SWITCH_THROW    = 0xE0,
    SWITCH_ACK      = 0xE2
} switch_code_t;

#define ACK_SUCCESS 0
#define ACK_FAILURE 1

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
