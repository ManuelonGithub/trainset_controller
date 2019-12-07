

#ifndef TRACK_SERVER_H
#define TRACK_SERVER_H

#include <stdint.h>
#include <stdbool.h>
#include "trainset_defs.h"

typedef struct route_ {
    direction_t     dir;
    bool            sw_req;
    switch_pos_t    sw_pos;
} route_t;

void track_server();
void track_init();


#endif // TRACK_SERVER_H
