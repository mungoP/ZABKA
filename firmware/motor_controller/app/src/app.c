#include "app/app.h"

#include <stdint.h>

#include "board/led.h"
#include "control/control.h"
#include "main.h"

#define HEARTBEAT_PERIOD_MS 500U

static uint32_t last_heartbeat_ms;

void app_init(void)
{
    (void)control_version();
    board_led_set(false);
    last_heartbeat_ms = HAL_GetTick();
}

void app_loop(void)
{
    const uint32_t now = HAL_GetTick();

    /* Non-blocking heartbeat; unsigned subtraction handles tick wraparound. */
    if ((now - last_heartbeat_ms) >= HEARTBEAT_PERIOD_MS)
    {
        last_heartbeat_ms = now;
        board_led_toggle();
    }
}
