/**
 * @file led.h
 * @brief Status LED on PD2 (net STATUS_LED).
 */
#ifndef BOARD_LED_H
#define BOARD_LED_H

#include <stdbool.h>

void board_led_set(bool on);
void board_led_toggle(void);

#endif /* BOARD_LED_H */
