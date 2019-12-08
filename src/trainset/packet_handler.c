


#include <stdio.h>
#include <string.h>
#include "packet_handler.h"
#include "frame_handler.h"
#include "trainset_defs.h"
#include "calls.h"
#include "cstr_utils.h"
#include "uart.h"

static packet_table_t   pkTable;    // Too big to be an automatic

void packet_server()
{
    packet_tracker_t tracker;

    initPacketServer(&tracker);

    pmbox_t box = bind(PACKET_BOX), src_box;

    uint8_t     rx_data[PACKET_META_SIZE+PACKET_DATA_MAX];
    size_t      rx_size;

    // received packet alias cast
    packet_t*   rx_pkt = (packet_t*)&rx_data;

    while(1) {
        rx_size = recv(box, ANY_BOX, rx_data, PACKET_DATA_MAX, &src_box);

        // Process Received message
        if (src_box == PACKET_BOX) {
            UART0_puts("[Packet Server] Packet Received (From Atmel).\n");
            processTrainsetPacket(rx_pkt, &tracker);
        }
        else {
            UART0_puts("[Packet Server] Message Receieved (From Track Server).\n");
            processControlMessage(rx_data, rx_size, src_box);
        }

        // Send Pending Packets
        while (tracker.Ns != pkTable.free &&
                sendPacket(&pkTable.packet[tracker.Ns], &tracker)) {}

        // Send an acknowledge packet if necessary
        if (tracker.pend_ack)   tracker.pend_ack = sendAck(&tracker);
    }
}

void initPacketServer(packet_tracker_t* tracker)
{
    pkTable.free = 0;
    pkTable.valid = 0;
    pkTable.full = 0;

    tracker->Nr = 0;
    tracker->Ns = 0;
    tracker->pend_ack = false;
}

void processTrainsetPacket(packet_t* pkt, packet_tracker_t* tracker)
{
    char num_buf[10];

    switch (pkt->ctrl.type) {
        case DATA: {
            UART0_puts("[Packet Server] Data Packet recv | Ns: ");
            UART0_puts(itoa(pkt->ctrl.Ns, num_buf));
            UART0_puts(" | Nr: ");
            UART0_puts(itoa(pkt->ctrl.Nr, num_buf));
            UART0_puts(" | Data: ");

            int i;
            for (i = 0; i < pkt->length; i++) {
                UART0_puts(itoa_hex(pkt->data[i], num_buf));
                UART0_puts(" ");
            }

            UART0_puts(" \n");

            if (tracker->Nr == pkt->ctrl.Ns) {
                PKT_MOV(tracker->Nr);
                send(TRACK_BOX, PACKET_BOX, pkt->data, pkt->length);

                flushPacketTable(pkt->ctrl.Nr);

                tracker->pend_ack = true;
            }
            else {
                UART0_puts("\n[Packet Server] Protocol Failure on RX!\n\n");
                sendNack(tracker);
            }
        } break;

        case ACK: {
            UART0_puts("[Packet Server] ACK packet recv: | ");
            UART0_puts("Nr: ");
            UART0_puts(itoa(tracker->Nr, num_buf));
            UART0_puts(" |\n");
            flushPacketTable(pkt->ctrl.Nr);
        } break;

        case NACK: {
            UART0_puts("[Packet Server] NACK packet recv: | ");
            UART0_puts("Nr: ");
            UART0_puts(itoa(tracker->Nr, num_buf));
            UART0_puts(" |\n");
            tracker->Ns = pkt->ctrl.Nr;
        } break;

        default: break;
    }
}

inline void flushPacketTable(uint8_t LastReceived)
{
    if (pkTable.valid != LastReceived) {
        // Going under the assumption last received is always lagging table's free pointer.
        while (pkTable.valid < LastReceived) {
            // Move last valid position
            PKT_MOV(pkTable.valid);
        }

        // Packets have been discarded, so there are packets available to be queued.
        pkTable.full = false;
    }
}

inline bool sendPacket(packet_t* pkt, packet_tracker_t* tracker)
{
    pkt->ctrl.Ns = tracker->Ns;
    pkt->ctrl.Nr = tracker->Nr;

    char num_buf[INT_BUF];

    if (startTransmission((char*)pkt, (PACKET_META_SIZE+pkt->length))) {
        // Data packet TX successful, no need to send any ACKs
        tracker->pend_ack = false;

        UART0_puts("[Packet Server] Data Packet send: | Ns: ");
        UART0_puts(itoa(tracker->Ns, num_buf));
        UART0_puts(" | Nr: ");
        UART0_puts(itoa(tracker->Nr, num_buf));
        UART0_puts(" | Data: ");

        int i;
        for (i = 0; i < pkt->length; i++) {
            UART0_puts(itoa_hex(pkt->data[i], num_buf));
            UART0_puts(" ");
        }

        UART0_puts(" \n");

        PKT_MOV(tracker->Ns);

        return true;
    }
    else {
        UART0_puts("[Packet Server] Data Packet send FAIL.\n");
    }

    return false;
}

inline bool sendAck(packet_tracker_t* tracker)
{
    packet_t pkt;

    pkt.ctrl.type = ACK;
    pkt.ctrl.Nr = tracker->Nr;
    pkt.length = 0;

    char num_buf[INT_BUF];

    if (startTransmission((char*)&pkt, (PACKET_META_SIZE+pkt.length))) {
        UART0_puts("[Packet Server] ACK packet send: | ");
        UART0_puts("Nr: ");
        UART0_puts(itoa(tracker->Nr, num_buf));
        UART0_puts(" |\n");
        return true;
    }
    else {
        UART0_puts("[Packet Server] ACK packet send FAIL.\n");
    }

    return false;
}

inline void sendNack(packet_tracker_t* tracker)
{
    packet_t pkt;
    pkt.ctrl.type = NACK;
    pkt.ctrl.Nr = tracker->Nr;
    pkt.length = 0;

    char num_buf[INT_BUF];

    UART0_puts("[Packet Server] NACK packet send: | ");
    UART0_puts("Nr: ");
    UART0_puts(itoa(tracker->Nr, num_buf));
    UART0_puts(" |\n");

    while (!startTransmission((char*)&pkt, (PACKET_META_SIZE+pkt.length))) {}
}

bool processControlMessage(uint8_t* data, size_t size, pmbox_t src_box)
{
    bool retval = false;

    packet_t* pkt;

    if (!pkTable.full) {
        // There's room in the table for a packet
        retval = true;

        pkt = &pkTable.packet[pkTable.free];

        memcpy(pkt->data, data, size);
        pkt->length = size;
        pkt->ctrl.type = DATA;

        PKT_MOV(pkTable.free);

        pkTable.full = (pkTable.free == pkTable.valid);
    }

    return retval;
}



