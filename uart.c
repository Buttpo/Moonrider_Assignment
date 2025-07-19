#include <reg51.h>
#include "uart.h"

void UART_Init(void) {
    TMOD |= 0x20;
    TH1 = 0xFD;
    TL1 = 0xFD;
    TR1 = 1;
    SCON = 0x50;
    TI = 1;

void UART_SendChar(char c) {
    while (!TI);
    TI = 0;
    SBUF = c;
}

void UART_SendString(char* str) {
    while (*str) {
        UART_SendChar(*str++);
    }
}
