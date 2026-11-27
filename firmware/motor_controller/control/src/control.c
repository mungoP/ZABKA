#include "control/control.h"

uint16_t control_version(void)
{
    return (uint16_t)((CONTROL_VERSION_MAJOR << 8) | CONTROL_VERSION_MINOR);
}
