

#include "track_manager.h"
#include "packet_handler.h"
#include "calls.h"
#include "cstr_utils.h"
#include "uart.h"
#include "route_table.h"


void trainset_controller()
{
    pmbox_t box = bind(TRACK_BOX);

    uint8_t rx_data[PACKET_DATA_MAX], tx_data[PACKET_DATA_MAX];

    train_msg_t* tx_msg = (train_msg_t*)tx_data;
    train_msg_t* rx_msg = (train_msg_t*)rx_data;

    train_t train;

    recv(12, TRACK_BOX, &train, sizeof(train_t), NULL);

    train.state = INIT;
}

void analyseRoute(route_t* route, train_t* train)
{

}
