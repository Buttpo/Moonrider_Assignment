#include <reg51.h>
#include "timer.h"
#include "uart.h"

unsigned char blink_counter = 0;
bit left_led_state = 0;
bit right_led_state = 0;
bit hazard_mode = 0;
unsigned char hazard_counter = 0;


sbit HEARTBEAT = P1^7;
sbit LEFT_BUTTON = P3^2;
sbit RIGHT_BUTTON = P3^3;
sbit LEFT_LED = P1^0;
sbit RIGHT_LED = P1^1;

unsigned char count = 0;
unsigned char left_press_counter = 0;
unsigned char right_press_counter = 0;
bit left_led_on = 0;
bit right_led_on = 0;

void check_buttons() {
    bit left_pressed = (LEFT_BUTTON == 0);
    bit right_pressed = (RIGHT_BUTTON == 0);

    if (left_pressed && right_pressed) {
        if (hazard_counter < 10) hazard_counter++;
        else if (hazard_counter == 10 && !hazard_mode) {
            hazard_mode = 1;
            left_led_on = 0;
            right_led_on = 0;
						UART_SendString("Hazard ON\r\n");
        }
    } else {
        hazard_counter = 0;
				
    }

    if (hazard_mode) {
        if ((left_pressed && !right_pressed) || (!left_pressed && right_pressed)) {
            static unsigned char exit_counter = 0;
            exit_counter++;
            if (exit_counter >= 10) {
                hazard_mode = 0;
                exit_counter = 0;
								UART_SendString("Hazard OFF\r\n");
            }
        } else {

            static unsigned char exit_counter = 0;
            exit_counter = 0;
        }
        return;
    }

    if (left_pressed) {
        if (left_press_counter < 10) {
            left_press_counter++;
        } else if (left_press_counter == 10) {
            left_press_counter++;

            if (!left_led_on) {
                left_led_on = 1;
                right_led_on = 0;
								UART_SendString("Left indicator ON\r\n");
            } else {
                left_led_on = 0;
								UART_SendString("Left indicator OFF\r\n");
            }
        }
    } else {
        left_press_counter = 0;
    }

    if (right_pressed) {
        if (right_press_counter < 10) {
            right_press_counter++;
        } else if (right_press_counter == 10) {
            right_press_counter++;

            if (!right_led_on) {
                right_led_on = 1;
                left_led_on = 0;
								UART_SendString("Right indicator ON\r\n");
            } else {
                right_led_on = 0;
								UART_SendString("Right indicator OFF \r\n");
            }
        }
    } else {
        right_press_counter = 0;
    }
}


void main() {
    HEARTBEAT = 0;
    LEFT_LED = 0;
    RIGHT_LED = 0;

    Timer0_Init();
		UART_Init();
		UART_SendString("System Ready\r\n");
    while (1) {
			if (Timer_Flag_100ms) {
					Timer_Flag_100ms = 0;

					count++;
					blink_counter++;

					if (count >= 10) {
							count = 0;
							HEARTBEAT = ~HEARTBEAT;
					}

					check_buttons();

					if (blink_counter >= 3) {
							blink_counter = 0;

							if (hazard_mode) {
									left_led_state = !left_led_state;
									right_led_state = left_led_state;
									LEFT_LED = left_led_state;
									RIGHT_LED = right_led_state;
							} else {
									if (left_led_on) {
											left_led_state = !left_led_state;
											LEFT_LED = left_led_state;
									} else {
											LEFT_LED = 0;
											left_led_state = 0;
									}

									if (right_led_on) {
											right_led_state = !right_led_state;
											RIGHT_LED = right_led_state;
									} else {
											RIGHT_LED = 0;
											right_led_state = 0;
									}
							}
					}

			}

    }
}
