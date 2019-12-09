

#ifndef TRACK_SERVER_H
#define TRACK_SERVER_H

#include <stdint.h>
#include <stdbool.h>
#include "trainset_defs.h"
#include "route_table.h"

void hall_sensor_test();

void trainset_controller();
void track_init();

inline uint8_t divertSwitch(uint8_t sw);
inline uint8_t resetSwitch(uint8_t sw);

inline targ_t driveTrain(uint8_t train, targ_t ctrl_arg);
inline void resetSensors();

void run_train(train_t* train);
void routeAnalysis(train_t* train);


#endif // TRACK_SERVER_H
