

#include "track_manager.h"
#include "packet_handler.h"
#include "calls.h"


void track_server()
{
    pmbox_t box = bind(TRACK_BOX);

    uint8_t rx_data[PACKET_DATA_MAX], tx_data[PACKET_DATA_MAX];

    trainset_msg_t* tx_msg = (trainset_msg_t*)tx_data;
    trainset_msg_t* rx_msg = (trainset_msg_t*)rx_data;

    uint8_t hall_queue = 0;

    while(1) {
        recv(TRACK_BOX, PACKET_BOX, rx_data, PACKET_DATA_MAX, NULL);

        if (rx_msg->code == SENSOR_TRIGGERED) {
            if (hall_queue != 0) {
                tx_msg->code = SENSOR_RESET;
                tx_msg->arg1 = hall_queue;
                tx_msg->arg2 = 0;

                send(PACKET_BOX, TRACK_BOX, tx_data, sizeof(trainset_msg_t));
            }

            hall_queue = rx_msg->arg1;
        }
    }
}
