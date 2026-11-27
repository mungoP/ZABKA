/**
 * @file control.h
 * @brief Hardware-free motor control core (pure C, no HAL).
 *
 * Everything under control/ must compile on both the STM32 and the host.
 * Do not include any STM32/HAL/CMSIS headers from this library.
 */
#ifndef CONTROL_CONTROL_H
#define CONTROL_CONTROL_H

#include <stdint.h>

#define CONTROL_VERSION_MAJOR 0U
#define CONTROL_VERSION_MINOR 1U

/** @return Control core version packed as (major << 8) | minor. */
uint16_t control_version(void);

#endif /* CONTROL_CONTROL_H */
