


#include <stdio.h>
#include <string.h>
#include "packet_handler.h"
#include "frame_handler.h"
#include "trainset_defs.h"
#include "calls.h"

static packet_table_t   pkTable;
static packet_tracker_t pkTracker;

void packet_server()
{
    initPacketServer();

    pmbox_t box = bind(PACKET_BOX), src_box;

    uint8_t     rx_data[PACKET_META_SIZE+PACKET_DATA_MAX];
    size_t      rx_size;

    // received packet alias cast
    packet_t*   rx_pkt = (packet_t*)&rx_data;

    packet_t dummy;
    dummy.ctrl.nr = 0;
    dummy.ctrl.ns = 0;
    dummy.ctrl.type = DATA;

    dummy.data[0] = 0xC0;
    dummy.data[1] = 0xFF;
    dummy.data[2] = 0x81;

    dummy.length = 3;

    while(1) {
        startTransmission((char*)&dummy, (PACKET_META_SIZE+dummy.length));
        rx_size = recv(box, ANY_BOX, (uint8_t*)&rx_pkt, sizeof(packet_t), &src_box);

        if (src_box == PACKET_BOX) {
            processTrainsetPacket(rx_pkt);
        }
        else {
            processControlMessage(rx_data, rx_size, src_box);
        }

        if (Packet)
    }
}

void initPacketServer()
{
    pkTracker.Nr = 0;
    pkTracker.Ns = 0;
    pkTable.free_ptr = 0;
    pkTable.valid_ptr = 0;
    pkTable.full = 0;
}

void processTrainsetPacket(packet_t* pkt)
{
    switch (pkt->ctrl.type) {
        case DATA: {
            if (pkTracker.Nr == pkt->ctrl.ns) {
                PKT_TRACK_MOV(pkTracker.Nr);
                send(TRACK_BOX, PACKET_BOX, pkt->data, pkt->length);

                flushPacketTable(pkt->ctrl.nr-1);
            }
            else {
                SendNack();
            }
        } break;

        case ACK: {
            flushPacketTable(pkt->ctrl.nr-1);
        } break;

        case NACK: {
            pkTracker.Ns = pkt->ctrl.nr;
        } break;

        default: break;
    }
}

inline void flushPacketTable(uint8_t LastReceived)
{
    packet_entry_t* pkt_e;

    while (pkTable.valid_ptr <= LastReceived) {
        pkt_e = &pkTable.entry[pkTable.valid_ptr];

        // Send confirmation message
        send(pkt_e->src_box, PACKET_BOX, pkt_e->packet.data, pkt_e->packet.length);

        // Move last valid position
        PKT_TRACK_MOV(pkTable.valid_ptr);
    }

    // Packets have been discarded, so there are packets available to be queued.
    pkTable.full = false;
}

inline void SendAck()
{
    packet_t pkt;

    pkt.ctrl.type = ACK;
    pkt.ctrl.nr = pkTracker.Nr;
    pkt.length = 0;

    startTransmission((char*)&pkt, (PACKET_META_SIZE+pkt.length));
}

inline void SendNack()
{
    packet_t pkt;

    pkt.ctrl.type = NACK;
    pkt.ctrl.nr = pkTracker.Nr;
    pkt.length = 0;

    startTransmission((char*)&pkt, (PACKET_META_SIZE+pkt.length));
}

bool processControlMessage(uint8_t* data, size_t size, pmbox_t src_box)
{
    bool retval = false;

    packet_entry_t* pkEntry;

    if (!pkTable.full) {
        // There's room in the table for a packet
        retval = true;

        pkEntry = &pkTable.entry[pkTable.free_ptr];

        pkEntry->src_box = src_box;

        memcpy(pkEntry->packet.length, data, size);
        pkEntry->packet.length = size;

        PKT_TRACK_MOV(pkTable.free_ptr);

        pkTable.full = (pkTable.free_ptr == pkTable.valid_ptr);
    }

    return retval;
}






