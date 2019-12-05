/**
* @file    frame_handler.h
* @brief   Contains the UART1 handler
* @author  Callum Cottrell, Emad Khan (Based on his work)
* @date    2019.09.18 (Created)
* @date    2019.10.03 (Last Modified)
*/

#ifndef UART_H
#define UART_H

#include "cirbuffer.h"

// UART0 & PORTA Registers
#define GPIO_PORTB_AFSEL_R  (*((volatile unsigned long *)0x40059420))   /// GPIOA Alternate Function Select Register
#define GPIO_PORTB_DEN_R    (*((volatile unsigned long *)0x4005951C))   /// GPIOA Digital Enable Register
#define GPIO_PORTB_PCTL_R   (*((volatile unsigned long *)0x4005952C))   /// GPIOA Port Control Register
#define UART1_DR_R          (*((volatile unsigned long *)0x4000D000))   /// UART0 Data Register
#define UART1_FR_R          (*((volatile unsigned long *)0x4000D018))   /// UART0 Flag Register
#define UART1_IBRD_R        (*((volatile unsigned long *)0x4000D024))   /// UART0 Integer Baud-Rate Divisor Register
#define UART1_FBRD_R        (*((volatile unsigned long *)0x4000D028))   /// UART0 Fractional Baud-Rate Divisor Register
#define UART1_LCRH_R        (*((volatile unsigned long *)0x4000D02C))   /// UART0 Line Control Register
#define UART1_CTL_R         (*((volatile unsigned long *)0x4000D030))   /// UART0 Control Register
#define UART1_IFLS_R        (*((volatile unsigned long *)0x4000D034))   /// UART0 Interrupt FIFO Level Select Register
#define UART1_IM_R          (*((volatile unsigned long *)0x4000D038))   /// UART0 Interrupt Mask Register
#define UART1_MIS_R         (*((volatile unsigned long *)0x4000D040))   /// UART0 Masked Interrupt Status Register
#define UART1_ICR_R         (*((volatile unsigned long *)0x4000D044))   /// UART0 Interrupt Clear Register
#define UART1_CC_R          (*((volatile unsigned long *)0x4000DFC8))   /// UART0 Clock Control Register

#define INT_VEC_UART1           6           // UART0 Rx and Tx interrupt index (decimal)
#define UART_FR_TXFF            0x00000020  // UART Transmit FIFO Full
#define UART_FR_RXFE            0x00000010  // UART Receive FIFO Empty
#define UART_FR_BUSY            0x00000008
#define UART_RX_FIFO_ONE_EIGHT  0x00000038  // UART Receive FIFO Interrupt Level at >= 1/8
#define UART_TX_FIFO_SVN_EIGHT  0x00000007  // UART Transmit FIFO Interrupt Level at <= 7/8
#define UART_LCRH_WLEN_8        0x00000060  // 8 bit word length
#define UART_LCRH_FEN           0x00000010  // UART Enable FIFOs
#define UART_CTL_UARTEN         0x00000301  // UART RX/TX Enable
#define UART_INT_TX             0x020       // Transmit Interrupt Mask
#define UART_INT_RX             0x010       // Receive Interrupt Mask
#define UART_INT_RT             0x040       // Receive Timeout Interrupt Mask
#define UART_CTL_EOT            0x00000010  // UART End of Transmission Enable
#define EN_RX_PB0               0x00000001  // Enable Receive Function on PA0
#define EN_TX_PB1               0x00000002  // Enable Transmit Function on PA1
#define EN_DIG_PB0              0x00000001  // Enable Digital I/O on PA0
#define EN_DIG_PB1              0x00000002  // Enable Digital I/O on PA1

#define SYSCTL_RCGCGPIO_R      (*((volatile unsigned long *)0x400FE608)) /// GPIO Clock Gating Register
#define SYSCTL_RCGCUART_R      (*((volatile unsigned long *)0x400FE618)) /// UART Clock Gating Register

#define SYSCTL_RCGCGPIO_UART1      0x00000002  // UART1 Clock Gating Control
#define SYSCTL_RCGCUART_GPIOB      0x00000002  // Port B Clock Gating Control

#define SYSCTRL_RCC_R           (*((volatile unsigned long *)0x400FE0B0)) /// Clock Configuration Register

#define CLEAR_USRSYSDIV     0xF83FFFFF  // Clear USRSYSDIV Bits
#define SET_BYPASS      0x00000800  // Set BYPASS Bit

#define NVIC_EN0_R      (*((volatile unsigned long *)0xE000E100))   /// Interrupt 0-31 Set Enable Register
#define NVIC_EN1_R      (*((volatile unsigned long *)0xE000E104))   /// Interrupt 32-54 Set Enable Register

#define NVIC_SYS_PRI1_R (*((volatile unsigned long *)0xE000E404))
#define UART_PRIORITY_LVL   0x00004000

#define UART0_ECHO_ON     true
#define UART0_ECHO_OFF    false

/**
 * @brief   UART descriptor structure
 * @details contains the rx and tx circular buffers
 *          and uart configuration information.
 */
typedef struct uart_descriptor_ {
    circular_buffer_t   tx;
    circular_buffer_t   rx;
    bool            echo;
} uart_descriptor_t;


#endif // UART_H
