

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
#define ROUTE_CW    ((0 << 24) | (0 << 16) | (1 << 8) | (CW))  // CW, speed = 10, sw = 0, pos = straight
#define ROUTE_CCW   ((0 << 24) | (0 << 16) | (1 << 8) | (CCW)) // CCW, speed = 10, sw = 0, pos = straight

// Special cases for Route Codes
#define ROUTE_CCW_S6    (DIVERTED << 24 | (6 << 16) | (1 << 8) | (CCW))
#define ROUTE_CCW_S5    (DIVERTED << 24 | (5 << 16) | (1 << 8) | (CCW))
#define ROUTE_CW_S4     (DIVERTED << 24 | (4 << 16) | (1 << 8) | (CW))
#define ROUTE_CCW_S3    (DIVERTED << 24 | (3 << 16) | (1 << 8) | (CCW))
#define ROUTE_CW_S2     (DIVERTED << 24 | (2 << 16) | (1 << 8) | (CW))
#define ROUTE_CW_S1     (DIVERTED << 24 | (1 << 16) | (1 << 8) | (CW))


typedef struct route_prog_ {
    direction_t dir;
    speed_t     speed;
    switch_t    sw;
    uint8_t     res;
} route_prog_t;

typedef union route_ {
    route_prog_t    prog;
    uint32_t        code;
} route_t;

route_t* accessRoute(uint8_t row, uint8_t column);

#endif // ROUTE_TABLE_H





