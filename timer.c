#include <reg51.h>
#include "timer.h"

bit Timer_Flag_100ms = 0;

void Timer0_Init(void) {
    TMOD &= 0xF0;
    TMOD |= 0x01;
    TH0 = 0x3C;
    TL0 = 0xB0;
    ET0 = 1;
    EA = 1;
    TR0 = 1;
}

void timer0_ISR(void) interrupt 1 {
    TH0 = 0x3C;
    TL0 = 0xB0;
    Timer_Flag_100ms = 1;
}
