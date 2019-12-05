/**
 * @file    frame_handler.c
 * @brief   Contains functionality to operate the UART0 driver for the tiva board.
 * @author  Callum Cottrell, Emad Khan (Based on his work)
 * @date    2019.09.18 (Created)
 * @date    2019.11.30 (Last Modified)
 */

#include <string.h>
#include "frame_handler.h"
#include "k_cpu.h"
#include "UART.h"
#include "k_types.h"
#include "k_messaging.h"

#define PACKET_BOX  14
void ioServerSend();
void discardBuffer();
void formPacket(char);
void transmitFrame();

enum recvStates {START,VALIDATE, ESCByte}; // these names suck im going to change them
enum sendStates {Wait, startTransmit, xmitPacket, ESC, ESC2, sendChecksum, sendETX};

int recvState;
int sendState;

int xmitLength;
int recvLength; // process that sends STX will also set this extern global
#define MAX_LENGTH 256 // no idea at the moment what max is\

int xmitChecksum;
int recvChecksum;
int badFrame;
char data;

static uart_descriptor_t UART1;

/**
 * @brief   Initializes the control registers for UART0 and the UART descriptor
 *          that is accessed by the driver.
 * @param   [in,out] descriptor: pointer to uart descriptor that will be accessed by the driver.
 * @param   [in] echo_en: Specifies if RX echo is enabled at driver level.
 * @param   [in] auto_flush_en: Specifies if driver should automatically send TX data if available.
 *
 * @todo    Convert the boolean configurations into a bit-style configuration.
 */
void UART1_init(uart_descriptor_t* descriptor)
{
    volatile int wait;

    /* Initialize UART0 */
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCUART_GPIOB;   // Enable Clock Gating for UART0
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCGPIO_UART1;   // Enable Clock Gating for PORTA
    wait = 0; // give time for the clocks to activate

    UART1_CTL_R &= ~UART_CTL_UARTEN;        // Disable the UART
    wait = 0;   // wait required before accessing the UART config regs

    // Setup the BAUD rate
    UART1_IBRD_R = 8;   // IBRD = int(16,000,000 / (16 * 115,200)) = 8.680555555555556
    UART1_FBRD_R = 44;  // FBRD = int(.680555555555556 * 64 + 0.5) = 44.05555555555556

    UART1_LCRH_R = (UART_LCRH_WLEN_8);  // WLEN: 8, no parity, one stop bit, without FIFOs)

    GPIO_PORTB_AFSEL_R = 0x3;        // Enable Receive and Transmit on PA1-0
    GPIO_PORTB_PCTL_R = (0x01) | ((0x01) << 4);         // Enable UART RX/TX pins on PA1-0
    GPIO_PORTB_DEN_R = EN_DIG_PB0 | EN_DIG_PB1;        // Enable Digital I/O on PA1-0

    UART1_CTL_R = UART_CTL_UARTEN;        // Enable the UART
    wait = 0; // wait; give UART time to enable itself.

    circular_buffer_init(&UART1.tx);
    circular_buffer_init(&UART1.rx);

    NVIC_SYS_PRI1_R = (UART_PRIORITY_LVL);

    UART1_InterruptEnable(INT_VEC_UART1);       // Enable UART0 interrupts
    UART1_IntEnable(UART_INT_RX | UART_INT_TX); // Enable Receive and Transmit interrupts
    recvState = START;
    sendState = startTransmit;
}

/**
 * @brief   Sets the interrupt enable bit for a peripheral in the NVIC register.
 * @param   [in] InterruptIndex:
 *              The peripheral's interrupt index in the NVIC register.
 */
void UART1_InterruptEnable(unsigned long InterruptIndex)
{
    /* Indicate to CPU which device is to interrupt */
    if(InterruptIndex < 32)
        NVIC_EN0_R = 1 << InterruptIndex;       // Enable the interrupt in the EN0 Register
    else
        NVIC_EN1_R = 1 << (InterruptIndex - 32);    // Enable the interrupt in the EN1 Register
}

/**
 * @brief   Sets bits in the UART0 interrupt mask register.
 * @param   [in] flags: Determines which bits will be set in the register based on its set bits.
 */
void UART1_IntEnable(unsigned long flags)
{
    /* Set specified bits for interrupt */
    UART1_IM_R |= flags;
}

/**
 * @brief   Interrupt Handler for UART1 (from atmel).
 */
void UART1_IntHandler(void)
{
    //Uncomment to test that the UART1 is initialized correctly
//    if (UART1_MIS_R & UART_INT_RX) {
//        /* RECV done - clear interrupt and make char available to application */
//        UART1_ICR_R |= UART_INT_RX;
//
//        enqueuec(&UART1->rx, UART1_DR_R);
//
//        ioServerSend();
//    }

    if (UART1_MIS_R & UART_INT_RX) {
        /* RECV done - clear interrupt and make char available to application */
        char c = UART1_DR_R;
        formPacket(c);
        UART0_put(&c, 1);
        UART1_ICR_R |= UART_INT_RX;
    }

    if (UART1_MIS_R & UART_INT_TX) {
        /* XMIT done - clear interrupt */
        UART1_ICR_R |= UART_INT_TX;
        transmitFrame();
    }

}

//TODO: i assume inline helps here for interrupt functions but idk
inline void formPacket(char c){

    //STX always forces first state.
    if (c == STX)
        recvState = START;

    switch (recvState){

    //Begin receiving a frame
    case START:

        //If length isnt 0 something went wrong with last frame
        if (xmitLength){
            discardBuffer();
            badFrame = 1;
        }

        xmitLength = 0;
        xmitChecksum = 0;
        recvState = VALIDATE;

        break;

    //Regular receive in the middle of a frame
    case VALIDATE:

        //If a DLE is received
        if (c == DLE)
            recvState = ESCByte;

        //End of Transmission
        else if (c == ETX){
            //Check xmitChecksum
            if (xmitChecksum != -1)
                discardBuffer();
            else
                sendPacket();

        }
        else if (xmitLength > MAX_LENGTH){
            discardBuffer();
        }
        else {
            enqueuec(&UART1.rx, c);
            xmitLength++;
            xmitChecksum += c;
        }

        break;

        // Take the next byte regardless
    case ESCByte:

        enqueuec(&UART1.rx, c);
        xmitLength++;
        xmitChecksum+=c;

        break;

    default:
        //for (;;)
     //   System.out.println("Youre in Java now >:)");
        break;
    }
}

inline void transmitFrame(){

   // data = peek_buffer(&UART1->tx);

    switch (sendState){

    case Wait:
        sendState = startTransmit;
        break;

    case startTransmit:
        xmitChecksum = 0;
        //STX already sent. thats why were in the UART1 handler.
        xmitLength = buffer_size(&UART1.tx); //might not need the length
           // if we use the bufferSize instead
        sendState = xmitPacket;

        //regular transmit
    case xmitPacket:
        //If this byte is a problem byte
        if (data == DLE || data == STX  || data == ETX ){
            sendState = ESC;
            UART1_DR_R = DLE;
        }
        //If the buffer has been fully transmitted prep checksum
        else if (!buffer_size(&UART1.tx)){
            xmitChecksum = ~xmitChecksum;
            if (xmitChecksum == DLE || xmitChecksum == STX || xmitChecksum == ETX)
                sendState = ESC2;
            else
                sendState = sendChecksum;
        }
        else {
            xmitChecksum += data;
            UART1_DR_R = dequeuec(&UART1.tx);
        }
        break;

    case ESC:
        sendState = xmitPacket;
        xmitChecksum += data;
        UART1_DR_R = dequeuec(&UART1.tx);
        break;
        //The escape that preludes checksum
    case ESC2:
        sendState = sendChecksum;
        UART1_DR_R = DLE;
        break;

    case sendChecksum:
        sendState = sendETX;
        UART1_DR_R = xmitChecksum;

        break;

    case sendETX:
        sendState = Wait;
        UART1_DR_R = ETX;

        break;
    default:

        break;
    }
}


//Dequeue everything, buffer was crap
void discardBuffer(){

    xmitLength = 0;
    xmitChecksum = 0;
    circular_buffer_init(&UART1.rx);
    badFrame = 1;
}

/**
 * @brief   Send a character to UART 0.
 * @param   [in] c: Character to be transmitted.
 * @details It sends a character to UART0's data register
 *          when the UART0 peripheral is ready to transmit.
 *          This function is blocking program progression
 *          while UART0 isn't ready to transmit.
 */
inline void UART1_putc(char c)
{
    while(!UART1_TxReady());
    UART1_DR_R = c;
}
/**
 *
 * @brief   Determines if UART 0 is ready to transmit.
 * @return  [bool] True if ready, false if busy.
 * @details TX ready is based in the BUSY flag in the UART 0's flag register.
 */
inline bool UART1_TxReady(void)
{
    return !(UART1_FR_R & UART_FR_BUSY);
}

bool startTransmission(char *packet, int length){

    if (sendState != startTransmit){

        memcpy(UART1.tx.data, packet, length);
        UART1.tx.rd_ptr = 0;
        UART1.tx.wr_ptr = length-1;

        UART1_DR_R = STX;

        return true;
    }
    //If its busy then enqueue it for later
    else {
        return false;
    }

}
/**
 * @brief   Send a character from the RX buffer to the kernel IO server.
 */
void sendPacket()
{
    pmsg_t msg = {
         .dst = PACKET_BOX,
         .src = PACKET_BOX,
         .data = (uint8_t*)&UART1.rx.data,
         .size = buffer_size(&UART1.rx)
    };

    k_MsgSend(&msg, NULL);
}

