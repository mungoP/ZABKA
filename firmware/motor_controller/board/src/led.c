#include "board/led.h"

#include "main.h"

/* PD2 is configured as a push-pull output by MX_GPIO_Init(). */
#define STATUS_LED_PORT GPIOD
#define STATUS_LED_PIN  GPIO_PIN_2

void board_led_set(bool on)
{
    HAL_GPIO_WritePin(STATUS_LED_PORT, STATUS_LED_PIN, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void board_led_toggle(void)
{
    HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN);
}
