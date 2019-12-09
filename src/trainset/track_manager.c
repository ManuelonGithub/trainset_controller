

#include "track_manager.h"
#include "packet_handler.h"
#include "calls.h"
#include "cstr_utils.h"
#include "uart.h"
#include "route_table.h"

/**
 * @brief   Trainset Controll Process program
 * @details Process function that operates the train track to maneuver a train to
 *          a set position.
 */
void trainset_controller()
{
    pmbox_t box = bind(TRACK_BOX);

    train_t train =
    {
         .current = 1,
         .dst_head = 7,
         .dst_tail = 8,
         .max = DEF_SPEED,
         .state = INIT,
         .sw_active = 0,
         .sw_queue = 0,
    };

    // Receive initializing data from user
//    recv(12, TRACK_BOX, &train, sizeof(train_t), NULL);

    // Initialize train state
    train.state = INIT;
    train.max = (train.max == 0) ? DEF_SPEED : train.max;    // Not allow user to set max speed to 0

    // Runs train until it reaches its set location
    run_train(&train);

    // reduce process' scheduling priority
    nice(LOWEST_PRIORITY);

    // Idle process
    while (1) {}
}

/**
 * @brief   diverts a switch.
 * @param   [in] sw: switch number to divert.
 * @return  switch # diverted.
 */
inline uint8_t divertSwitch(uint8_t sw)
{
    train_msg_t msg = {.code = SWITCH_THROW, .arg1 = sw, .arg2 = DIVERTED};

    send(PACKET_BOX, TRACK_BOX, (uint8_t*)&msg, sizeof(train_msg_t));

    return sw;
}

/**
 * @brief   Resets a switch.
 * @param   [in] sw: switch number to reset.
 * @return  switch # resetted.
 */
inline uint8_t resetSwitch(uint8_t sw)
{
    train_msg_t msg = {.code = SWITCH_THROW, .arg1 = sw, .arg2 = STRAIGHT};
    send(PACKET_BOX, TRACK_BOX, (uint8_t*)&msg, sizeof(train_msg_t));

    return sw;
}

/**
 * @brief   Drives a train# according to a control argument.
 * @param   [in] train: train # to drive.
 * @param   [in] ctrl_arg: Contains the control data sequence necessary to drive the train.
 * @return  the control argument sent.
 */
inline targ_t driveTrain(uint8_t train, targ_t ctrl_arg)
{
    train_msg_t msg = {.code = TRAIN_MOVE, .arg1 = train, .arg2 = ctrl_arg};
    send(PACKET_BOX, TRACK_BOX, (uint8_t*)&msg, sizeof(train_msg_t));

    return ctrl_arg;
}

/**
 * @brief   resets all hall sensors triggered.
 */
inline void resetSensors()
{
    train_msg_t msg = {.code = SENSOR_RESET, .arg1 = 0xFF, .arg2 = 0};
    send(PACKET_BOX, TRACK_BOX, (uint8_t*)&msg, sizeof(train_msg_t));
}

/**
 * @brief   Runs the train control sequence until the train reaches its set destination.
 * @param   [in, out] train:
 *              pointer to train structure that contains
 *              all info and data required to run train.
 */
void run_train(train_t* train)
{
    uint8_t rx_data[PACKET_DATA_MAX];
    train_msg_t* rx_msg = (train_msg_t*)rx_data;

    // Set the train state to be moving
    train->state = MOVING;

    while (train->state != DESTINATION) {
        // Analyze route to know in which direction to go & speed, and if destination has been reached.
        routeAnalysis(train);

//        send(PACKET_BOX, TRACK_BOX, (uint8_t*)&msg, sizeof(train_msg_t));

        // Receive messages from the train track
        recv(TRACK_BOX, PACKET_BOX, rx_data, PACKET_DATA_MAX, NULL);

//        request(PACKET_BOX, TRACK_BOX, (uint8_t*)&msg, sizeof(train_msg_t), rx_data, PACKET_DATA_MAX);

        if (rx_msg->code == SENSOR_TRIGGERED) {
            // Train has moved past a hall sensor
            // Update the train's current sensor location
            train->current = rx_msg->arg1;
//            send(USER_BOX, TRACK_BOX, &train->current, 1);
        }
    }
}

/**
 * @brief   Analysis the current route of a train
 * @param   [in, out] train: Pointer to train structure whose route will be analyzed.
 */
void routeAnalysis(train_t* train)
{
    // Retrieve current route from table
    route_t* route = (train->current == train->dst_tail) ?
            accessRoute(train->current-1, train->dst_head-1) :  // If passed the tail of the destination
            accessRoute(train->current-1, train->dst_tail-1);

    // Set up train speed and direction
    if (route->code == DONE) {
        // Train's destination has been reached
        train->state = DESTINATION;
        train->ctrl.arg = BREAK;
        // reset sensors?
    }
    else {
        train->ctrl.dir = route->prog.dir;
        train->ctrl.mag = train->max;
    }

    // Set up switch configuration
    if (route->prog.sw != 0) {
        divertSwitch(route->prog.sw);

        if (train->sw_queue != 0)   resetSwitch(train->sw_queue);
        if (train->sw_active != 0)  train->sw_queue = train->sw_active;

        train->sw_active = route->prog.sw;
    }

    driveTrain(0xFF, train->ctrl.arg);
}
