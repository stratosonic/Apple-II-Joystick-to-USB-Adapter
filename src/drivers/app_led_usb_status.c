#include "stdint.h"
#include "system.h"
#include "usb_device.h"

void APP_LEDUpdateUSBStatus(void) {
    static uint16_t ledCount = 0;

    // If device is suspended, LED is turned off
    if (USBIsDeviceSuspended() == true) {
        LED_Off(LED_USB_DEVICE_STATE);
        return;
    }

    switch (USBGetDeviceState()) {
        // If device is configured, LED on
        case CONFIGURED_STATE: 
            LED_On(LED_USB_DEVICE_STATE);
            break;

        default:
            // Not configured so slow blink
            if (ledCount == 1) {
                LED_On(LED_USB_DEVICE_STATE);
            } else if (ledCount == 50) {
                LED_Off(LED_USB_DEVICE_STATE);
            } else if (ledCount > 950) {
                ledCount = 0;
            }
            break;
    }

    /* Increment the millisecond counter. */
    ledCount++;
}