

#ifndef ROUTE_TABLE_H
#define ROUTE_TABLE_H

#include <stdint.h>
#include <stdbool.h>
#include "trainset_defs.h"

#define HALL_SENSORS 24

#define NO_SWITCH   0
#define BREAK       0

// Route Codes: Little endian aligned!
#define DONE        0x00000000  // CW, speed = 0, sw = 0, pos = straight
#define CW_ROUTE    (STRAIGHT << 24 | (NO_SWITCH << 16) | (DEF_SPEED << 8) | (CW))  // CW, speed = 10, sw = 0, pos = straight
#define CCW_ROUTE   (STRAIGHT << 24 | (NO_SWITCH << 16) | (DEF_SPEED << 8) | (CCW)) // CCW, speed = 10, sw = 0, pos = straight

// Special cases for Route Codes
#define ROUTE_1_17      ((DIVERTED << 24) | (6 << 16) | (DEF_SPEED << 8) | (CCW))   // CCW, speed = 10, sw = 6, pos = diverted
#define ROUTE_1_CCW     ((STRAIGHT << 24) | (6 << 16) | (DEF_SPEED << 8) | (CCW))   // CCW, speed = 10, sw = 6, pos = straight

#define ROUTE_3_21      ((DIVERTED << 24) | (5 << 16) | (DEF_SPEED << 8) | (CCW))   // CCW, speed = 10, sw = 5, pos = diverted
#define ROUTE_3_CCW     ((STRAIGHT << 24) | (5 << 16) | (DEF_SPEED << 8) | (CCW))   // CCW, speed = 10, sw = 5, pos = diverted

#define ROUTE_6_18      ((DIVERTED << 24) | (4 << 16) | (DEF_SPEED << 8) | (CW))    // CW, speed = 10, sw = 4, pos = diverted
#define ROUTE_6_CW      ((STRAIGHT << 24) | (4 << 16) | (DEF_SPEED << 8) | (CW))    // CW, speed = 10, sw = 4, pos = diverted

#define ROUTE_9_19      ((DIVERTED << 24) | (3 << 16) | (DEF_SPEED << 8) | (CCW))   // CCW, speed = 10, sw = 3, pos = diverted
#define ROUTE_9_CCW     ((STRAIGHT << 24) | (3 << 16) | (DEF_SPEED << 8) | (CCW))   // CCW, speed = 10, sw = 3, pos = diverted

#define ROUTE_11_23     ((DIVERTED << 24) | (2 << 16) | (DEF_SPEED << 8) | (CCW))   // CW, speed = 10, sw = 4, pos = diverted
#define ROUTE_11_CCW    ((STRAIGHT << 24) | (2 << 16) | (DEF_SPEED << 8) | (CCW))   // CW, speed = 10, sw = 4, pos = diverted

#define ROUTE_14_20     ((DIVERTED << 24) | (1 << 16) | (DEF_SPEED << 8) | (CW))    // CW, speed = 10, sw = 4, pos = diverted
#define ROUTE_14_CW     ((STRAIGHT << 24) | (1 << 16) | (DEF_SPEED << 8) | (CW))    // CW, speed = 10, sw = 4, pos = diverted


typedef struct route_prog_ {
    direction_t dir;
    speed_t     speed;
    switch_t    sw;
    sw_pos_t    res;
} route_prog_t;

typedef union route_ {
    route_prog_t    prog;
    uint32_t        code;
} route_t;

route_t* table_access(uint8_t row, uint8_t column);

#endif // ROUTE_TABLE_H





