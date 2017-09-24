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

/**
 * Apple II Joystick to USB Adapter
 * www.flitey.com
 */

/** INCLUDES *******************************************************/
#include "system.h"

#include "usb.h"
#include "usb_device_hid.h"

#include "joystick.h"
#include "app_led_usb_status.h"

#include "eeprom.h"

// Define default EEPROM data for min/max calibration values
//             [ X MIN  ]  [ X MID  ]  [ X MAX  ]  [ Y MIN  ]
__EEPROM_DATA(0x00, 0x00, 0x01, 0xFF, 0x03, 0xFF, 0x00, 0x00);

//             [ Y MID  ]  [ Y MAX  ]
__EEPROM_DATA(0x01, 0xFF, 0x03, 0xFF, 0x00, 0x00, 0x00, 0x00);

/**
 * Main function
 */
void main(void) {

    // Initialize the buttons, LEDs, ADC
    SYSTEM_Initialize(SYSTEM_STATE_USB_START);

    // Check button states
    bool isSwitch0Pressed = BUTTON_IsPressed(GAME_SWITCH_0);
    bool isSwitch1Pressed = BUTTON_IsPressed(GAME_SWITCH_1);

    // If both switches are pressed, we will reset the calibration to the 
    // standard values.
    if (isSwitch0Pressed == true && isSwitch1Pressed == true) {

        // First set the values to be used
        JoystickUseSavedCalibration(JOYSTICK_10_BIT_MIN, JOYSTICK_10_BIT_MID,
                JOYSTICK_10_BIT_MAX, JOYSTICK_10_BIT_MIN, JOYSTICK_10_BIT_MID,
                JOYSTICK_10_BIT_MAX);

        // And then save them to the EEPROM
        EepromWriteTwoBytes(X_AXIS_MIN_EEPROM_ADDRESS, JOYSTICK_10_BIT_MIN);
        EepromWriteTwoBytes(X_AXIS_MID_EEPROM_ADDRESS, JOYSTICK_10_BIT_MID);
        EepromWriteTwoBytes(X_AXIS_MAX_EEPROM_ADDRESS, JOYSTICK_10_BIT_MAX);

        EepromWriteTwoBytes(Y_AXIS_MIN_EEPROM_ADDRESS, JOYSTICK_10_BIT_MIN);
        EepromWriteTwoBytes(Y_AXIS_MID_EEPROM_ADDRESS, JOYSTICK_10_BIT_MID);
        EepromWriteTwoBytes(Y_AXIS_MAX_EEPROM_ADDRESS, JOYSTICK_10_BIT_MAX);

    } else if (isSwitch0Pressed == true || isSwitch1Pressed == true) {
        // If only one of the switches is pressed, we will run the calibration
        // routine to get the new values and save them to EEPROM

        JoystickRunCalibration();

    } else {
        // Otherwise, we will just retrieve the previous calibration values and
        // run with those.
        uint16_t x_min = EepromReadTwoBytes(X_AXIS_MIN_EEPROM_ADDRESS);
        uint16_t x_mid = EepromReadTwoBytes(X_AXIS_MID_EEPROM_ADDRESS);
        uint16_t x_max = EepromReadTwoBytes(X_AXIS_MAX_EEPROM_ADDRESS);

        uint16_t y_min = EepromReadTwoBytes(Y_AXIS_MIN_EEPROM_ADDRESS);
        uint16_t y_mid = EepromReadTwoBytes(Y_AXIS_MID_EEPROM_ADDRESS);
        uint16_t y_max = EepromReadTwoBytes(Y_AXIS_MAX_EEPROM_ADDRESS);

        JoystickUseSavedCalibration(x_min, x_mid, x_max, y_min, y_mid, y_max);
    }

    // Initialize USB
    USBDeviceInit();

    // Attach USB
    USBDeviceAttach();

    // Now just loop forever here
    while (1) {

        // We are using USB_POLLING so do USB tasks
        USBDeviceTasks();

        // If we are not configured, break from the loop
        if (USBGetDeviceState() < CONFIGURED_STATE) {
            continue;
        }

        // If we are suspended, break from the loop
        if (USBIsDeviceSuspended() == true) {
            continue;
        }

        // Perform joystick related actions
        JoystickTasks();
    }
}

