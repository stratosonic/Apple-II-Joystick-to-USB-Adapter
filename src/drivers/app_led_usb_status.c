/*
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License.
 If not, see <http://www.gnu.org/licenses/>.
 */

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