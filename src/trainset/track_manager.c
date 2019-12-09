

#include "track_manager.h"
#include "packet_handler.h"
#include "calls.h"
#include "cstr_utils.h"
#include "uart.h"



void trainset_controller()
{
    pmbox_t box = bind(TRACK_BOX);

    uint8_t rx_data[PACKET_DATA_MAX], tx_data[PACKET_DATA_MAX];

    trainset_msg_t* tx_msg = (trainset_msg_t*)tx_data;
    trainset_msg_t* rx_msg = (trainset_msg_t*)rx_data;

    train_state_t state;

    recv(12, TRACK_BOX, &state, sizeof(train_state_t), NULL);
}
