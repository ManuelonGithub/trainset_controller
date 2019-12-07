

#ifndef ROUTE_TABLE_H
#define ROUTE_TABLE_H

#include <stdint.h>
#include <stdbool.h>
#include "trainset_defs.h"

#define HALL_SENSORS 24

#define NO_SWITCH   0
#define BREAK       0

typedef struct route_ {
    direction_t dir;
    speed_t     speed;
    switch_t    sw;
    sw_pos_t    pos;
} route_t;


#endif // ROUTE_TABLE_H





