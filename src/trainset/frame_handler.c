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

#include "k_types.h"
#include "k_messaging.h"

void ioServerSend();
void discardBuffer();

enum recvStates {start, validate, ESCByte}; // these names suck im going to change them
int recvState;

int length;
#define MAX_LENGTH 100 // no idea at the moment what max is
int checksum;
int badFrame;

static uart_descriptor_t* UART1;

/**
 * @brief   Initializes the control registers for UART0 and the UART descriptor
 *          that is accessed by the driver.
 * @param   [in,out] descriptor: pointer to uart descriptor that will be accessed by the driver.
 * @param   [in] echo_en: Specifies if RX echo is enabled at driver level.
 * @param   [in] auto_flush_en: Specifies if driver should automatically send TX data if available.
 *
 * @todo    Convert the boolean configurations into a bit-style configuration.
 */
void UART1_Init(uart_descriptor_t* descriptor)
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

    UART1 = descriptor;

    circular_buffer_init(&UART1->tx);
    circular_buffer_init(&UART1->rx);

    NVIC_SYS_PRI1_R = (UART_PRIORITY_LVL);

    UART0_InterruptEnable(INT_VEC_UART1);       // Enable UART0 interrupts
    UART0_IntEnable(UART_INT_RX | UART_INT_TX); // Enable Receive and Transmit interrupts
    recvState = start;
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
        UART1_ICR_R |= UART_INT_RX;
        receivePacket();
        //ioServerSend();
    }

    if (UART1_MIS_R & UART_INT_TX) {
        /* XMIT done - clear interrupt */
        UART1_ICR_R |= UART_INT_TX;
    }

}

//TODO: i assume inline helps here for interrupt functions but idk
inline void receivePacket(){

    //STX always forces first state.
    if (UART1_DR_R == STX)
        recvState = start;

    switch (recvState){

    //Begin receiving a frame
    case start:

        //If length isnt 0 something went wrong with last frame
        if (length){
            discardBuffer();
            badFrame = 1;
        }

        length = 0;
        checksum = 0;
        recvState = validate;

        break;

    //Regular receive in the middle of a frame
    case validate:

        //If a DLE is received
        if (UART1_DR_R == DLE)
            recvState = ESCByte;

        //End of Transmission
        else if (UART1_DR_R == ETX){
            //Check checksum
            if (checksum != -1)
                discardBuffer();
            //else
                //Send the buffer here? IPC?
        }
        else if (length > MAX_LENGTH){
            discardBuffer();
        }
        else {
            enqueuec(&UART1->rx, UART1_DR_R);
            length++;
            checksum += UART1_DR_R;
        }

        break;

        // Take the next byte regardless
    case ESCByte:

        enqueuec(&UART1->rx, UART1_DR_R);
        length++;
        checksum+=UART1_DR_R;

        break;

    default:
     //   System.out.println("Youre in Java now >:)");

    }
}

//Dequeue everything, buffer was crap
void discardBuffer(){

    length = 0;
    checksum = 0;
    //TODO: i dont know how your thingy works
    while (&UART1->rx){
        dequeuec(&UART1->rx);
    }
    badFrame = 1;
}
