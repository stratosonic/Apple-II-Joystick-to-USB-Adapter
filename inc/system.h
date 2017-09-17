/**
 * System definitions and functions
 */

#ifndef SYSTEM_H
#define SYSTEM_H

#include <xc.h>
#include <stdbool.h>

#include "buttons.h"
#include "leds.h"
#include "adc.h"

#include "io_mapping.h"
#include "fixed_address_memory.h"

/*** System States **************************************************/
typedef enum {
    SYSTEM_STATE_USB_START,
    SYSTEM_STATE_USB_SUSPEND,
    SYSTEM_STATE_USB_RESUME
} SYSTEM_STATE;

void SYSTEM_Initialize(SYSTEM_STATE state);

#endif
