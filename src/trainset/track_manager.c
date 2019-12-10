

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
         .current = 0,
         .dst = 2,
         .max = DEF_SPEED,
         .state = INIT,
         .sw_active = 0
    };

    // Receive initializing data from user
//    recv(12, TRACK_BOX, &train, sizeof(train_t), NULL);

    // Initialize train state
    train.state = INIT;
    train.max = (train.max == 0) ? DEF_SPEED : train.max;    // Not allow user to set max speed to 0

    // Runs train until it reaches its set location

//    run_train(&train);

    // reduce process' scheduling priority
    nice(LOWEST_PRIORITY);

    // Idle process
    train.ctrl.dir = CW;
    train.ctrl.mag = DEF_SPEED;

    driveTrain(0xFF, train.ctrl.arg);

    uint8_t rx_data[PACKET_DATA_MAX];
    train_msg_t* rx_msg = (train_msg_t*)rx_data;

    while (1) {
        recv(TRACK_BOX, PACKET_BOX, rx_data, PACKET_DATA_MAX, NULL);

        if (rx_msg->code == SENSOR_TRIGGERED) {
            if (rx_msg->arg1 == 10) {
                driveTrain(0xFF, BREAK);

                train.ctrl.dir = CCW;
                driveTrain(0xFF, train.ctrl.arg);
            }

            if (train.current != 0) {
                resetSensors(train.current);
            }
            train.current = rx_msg->arg1;
        }
    }
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
inline void resetSensors(uint8_t sensor)
{
    train_msg_t msg = {.code = SENSOR_RESET, .arg1 = sensor, .arg2 = 0};
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

    char num_buf[INT_BUF];

    // Set the train state to be moving
    train->state = MOVING;

    while (train->state != DESTINATION) {
        // Analyze route to know in which direction to go & speed, and if destination has been reached.
        routeAnalysis(train);

//        send(PACKET_BOX, TRACK_BOX, (uint8_t*)&msg, sizeof(train_msg_t));

        rx_msg->code = 0;
        while (rx_msg->code != SENSOR_TRIGGERED) {
            // Receive messages from the train track
            recv(TRACK_BOX, PACKET_BOX, rx_data, PACKET_DATA_MAX, NULL);

        }
//        request(PACKET_BOX, TRACK_BOX, (uint8_t*)&msg, sizeof(train_msg_t), rx_data, PACKET_DATA_MAX);

        // Train has moved past a hall sensor
        // Update the train's current sensor location
        train->current = rx_msg->arg1;

//        UART0_puts("Train is at sensor #");
//        UART0_puts(itoa(rx_msg->arg1, num_buf));
//        UART0_puts("\n");
    }
}

/**
 * @brief   Analysis the current route of a train
 * @param   [in, out] train: Pointer to train structure whose route will be analyzed.
 */
inline void routeAnalysis(train_t* train)
{
    // Retrieve current route from table
    route_t* route = accessRoute(train->current-1, train->dst-1);

    // Set up train speed and direction
    if (route->code == DONE) {
        // Train's destination has been reached
        train->state = DESTINATION;
        train->ctrl.arg = BREAK;
//        driveTrain(0xFF, train->ctrl.arg);
        // reset sensors?
//        driveTrain(0xFF, train->ctrl.arg);
        UART0_puts("Train at destination!!\n\n");
    }
    else {
        train->ctrl.dir = route->prog.dir;
        train->ctrl.mag = train->max;
//        driveTrain(0xFF, train->ctrl.arg);
    }

    driveTrain(0xFF, train->ctrl.arg);

    // Set up switch configuration
    if (route->prog.sw != 0) {
        divertSwitch(route->prog.sw);

        // Resolve the switch reset queueing
        if (train->sw_active != 0)  resetSwitch(train->sw_active);

        train->sw_active = route->prog.sw;
    }
}
