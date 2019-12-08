

#include "track_manager.h"
#include "packet_handler.h"
#include "calls.h"
#include "cstr_utils.h"
#include "uart.h"

void hall_sensor_test()
{
    uint8_t data[64];
    uint8_t sensor = 0;

    packet_t* pkt = (packet_t*)data;

    pkt->data[0] = SENSOR_TRIGGERED;

    int i;

    while(sensor < 6) {
        for (i = 0; i < 65536*2; i++) {}

        sensor++;
        pkt->data[1] = sensor;

        startTransmission((char*)data, 5);
    }
}

void track_server()
{
    pmbox_t box = bind(TRACK_BOX);

    uint8_t rx_data[PACKET_DATA_MAX], tx_data[PACKET_DATA_MAX];

    trainset_msg_t* tx_msg = (trainset_msg_t*)tx_data;
    trainset_msg_t* rx_msg = (trainset_msg_t*)rx_data;

    uint8_t hall_queue = 0;

    train_ctrl_t train_status = {.dir = CCW, .mag = 0, .res = 0};

    tx_msg->code = TRAIN_MOVE;
    tx_msg->arg1 = 0xFF;
    tx_msg->arg2 = *(targ_t*)(&train_status);

    char num_buf[INT_BUF];

    bool done = false;

    // UART0_puts("[Track Server] Sending Train move message.\n");
    send(PACKET_BOX, TRACK_BOX, tx_data, sizeof(trainset_msg_t));

    while(1) {
        recv(TRACK_BOX, PACKET_BOX, rx_data, PACKET_DATA_MAX, NULL);

        // UART0_puts("[Track Server] Received Message from Atmel.\n");

        if (rx_msg->code == SENSOR_TRIGGERED) {
            // UART0_puts("[Track Server] Train passed sensor #");
            // UART0_puts(itoa(rx_msg->arg1, num_buf));
            // UART0_puts("\n\n");

            if (rx_msg->arg1 == 18) {
                train_status.mag = 15;
                tx_msg->code = TRAIN_MOVE;
                tx_msg->arg1 = 0xFF;
                tx_msg->arg2 = 0;

                // UART0_puts("[Track Server] Stopping Train.\n\n");
                send(PACKET_BOX, TRACK_BOX, tx_data, sizeof(trainset_msg_t));
                done = true;
            }
        }
        else if (rx_msg->code == TRAIN_ACK) {
            // UART0_puts("[Track Server] Train move ack message received.\n");
            if (done == true) {
                tx_msg->code = SENSOR_RESET;
                tx_msg->arg1 = 0xFF;
                tx_msg->arg2 = 0;

                // UART0_puts("[Track Server] Resetting Sensors.\n\n");
                send(PACKET_BOX, TRACK_BOX, tx_data, sizeof(trainset_msg_t));
            }
        }
        else if (rx_msg->code == SENSOR_RST_ACK) {
            // UART0_puts("[Track Server] Sensor #");
            // UART0_puts(itoa(rx_msg->arg1, num_buf));
            // UART0_puts(" Reset msg ");

            if (rx_msg->arg2 == 1) {
                // UART0_puts("Sucessful.\n\n");
            }
            else {
                // UART0_puts("Failed. \n\n");
            }
        }
    }
}
