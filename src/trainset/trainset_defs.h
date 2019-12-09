

#ifndef TRAINSET_DEFINITIONS_H
#define TRAINSET_DEFINITIONS_H

#include <stdint.h>
#include <stdbool.h>

#define PACKET_BOX          14
#define TRACK_BOX           13

#define PACKET_NAME "Packet Server"
#define TRACK_NAME  "Track Manager"
#define TRAIN_NAME  "Train Manager"

typedef uint8_t direction_t;
typedef uint8_t sw_pos_t;
typedef uint8_t speed_t;
typedef uint8_t switch_t;
typedef uint8_t targ_t;

#define DEF_SPEED   10

enum TRAIN_DIRECTIONS {CW, CCW};
enum SWITCH_POSITIONS {STRAIGHT, DIVERTED};

typedef uint8_t code_t;
typedef code_t sensor_code_t;
typedef code_t train_code_t;
typedef code_t switch_code_t;

enum SENSOR_CODES {
    SENSOR_TRIGGERED    = 0xA0,
    SENSOR_TRIG_ACK     = 0xA2,
    SENSOR_RESET        = 0xA8,
    SENSOR_RST_ACK      = 0xAA
};

enum TRAIN_CODES {
    TRAIN_MOVE    = 0xC0,
    TRAIN_ACK     = 0xC2
};

enum SWITCH_CODES {
    SWITCH_THROW    = 0xE0,
    SWITCH_ACK      = 0xE2
};

#define ACK_SUCCESS 0
#define ACK_FAILURE 1

typedef struct train_msg_ {
    code_t code;
    targ_t arg1;
    targ_t arg2;
} train_msg_t;

typedef union train_ctrl_ {
    struct {
        speed_t     mag : 4;
        uint8_t     res : 3;
        direction_t dir : 1;
    };
    targ_t arg;
} train_ctrl_t;

typedef enum TRAIN_STATES {INIT, MOVING, DESTINATION} train_state_t;

typedef struct train_ {
    train_ctrl_t    ctrl;
    speed_t         max;
    uint8_t         dst_tail;
    uint8_t         dst_head;
    uint8_t         current;
    train_state_t   state;
    uint8_t         sw_active;
    uint8_t         sw_queue;
} train_t;

#define PACKET_MAX          8
#define PACKET_DATA_MAX     256

#define PACKET_META_SIZE    2
#define PACKET_MAX_SIZE     PACKET_META_SIZE+PACKET_DATA_MAX

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

typedef struct packet_table_ {
    packet_t    packet[PACKET_MAX];
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

#endif // TRAINSET_DEFINITIONS_H
