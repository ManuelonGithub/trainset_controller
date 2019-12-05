


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "packet_handler.h"

void packet_server()
{
    packet_table_t pkTable;
    packet_t ack_pkt;

    initPacketServer(&pkTable, &ack_pkt);

    pmbox_t box = bind(PACKET_BOX), src_box;

    packet_t rx_pkt;

    packet_t dummy;
    dummy.ctrl.nr = 1;
    dummy.ctrl.ns = 0;
    dummy.ctrl.type = DATA;

    dummy.data[0] = 0xC0;
    dummy.data[1] = 0xFF;
    dummy.data[2] = 0x‭81‬;

    dummy.length = 3;

    while(1) {
        recv(box, ANY_BOX, (uint8_t*)&rx_pkt, sizeof(packet_t), &src_box);

        if (src_box == PACKET_BOX) {
            startTransmission((uint8_t*)&rx_pkt, (PACKET_META_SIZE+dummy.length));
        }
    }
}

void initPacketServer(packet_table_t* table, packet_t* ack)
{
    table->Nr = 0;
    table->Ns = 0;
    table->last_valid = 0;

    int i;
    for (i = 0; i < PACKET_MAX; i++) {
        table->packet[i].valid = false;
        table->packet[i].src_box = ANY_BOX;
    }

    ack->ctrl.type = ACK;
    ack->ctrl.ns = 0;
}
