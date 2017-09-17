/**
 * Joystick functions
 * www.flitey.com
 */

#ifndef USBJOYSTICK_C
#define USBJOYSTICK_C

/** INCLUDES *******************************************************/
#include "usb.h"
#include "usb_device_hid.h"
#include "system.h"
#include "app_led_usb_status.h"
#include "stdint.h"
#include "system.h"
#include "eeprom.h"

/** DECLARATIONS ***************************************************/
uint8_t joystick_counter = 0;
bool calibrated = false;
uint16_t x_axis_center = JOYSTICK_10_BIT_MID;
uint16_t y_axis_center = JOYSTICK_10_BIT_MID;
uint16_t x_axis_max = JOYSTICK_10_BIT_MID;
uint16_t x_axis_min = JOYSTICK_10_BIT_MID;
uint16_t y_axis_max = JOYSTICK_10_BIT_MID;
uint16_t y_axis_min = JOYSTICK_10_BIT_MID;

/** TYPE DEFINITIONS ************************************************/
typedef union _INTPUT_CONTROLS_TYPEDEF {

    struct {

        struct {
            uint8_t x : 1;
            uint8_t y : 1;
uint8_t:
            6; //filler
        } buttons;

        struct {
            uint8_t X;
            uint8_t Y;
        } analog_stick;
    } members;
    uint8_t val[3];
} INPUT_CONTROLS;

INPUT_CONTROLS joystick_input @ JOYSTICK_DATA_ADDRESS;

USB_VOLATILE USB_HANDLE lastTransmission = 0;

void JoystickInitialize(void) {
    lastTransmission = 0;

    //enable the HID endpoint
    USBEnableEndpoint(JOYSTICK_EP, USB_IN_ENABLED | USB_HANDSHAKE_ENABLED | USB_DISALLOW_SETUP);
}

/**
 * Calibrate the joystick potentiometers. We wait for the button to be released 
 * and then start the calibration. Calibration looks for the min and max values
 * for each potentiometer and also checks if one of the buttons has been pressed.
 * The button press indicates calibration is complete.
 */
void JoystickRunCalibration() {

    while (BUTTON_IsPressed(GAME_SWITCH_0) == true || BUTTON_IsPressed(GAME_SWITCH_1) == true) {
        // wait for buttons to be released
    }

    uint16_t ledCounter = 0;
    while (!calibrated) {

        if (ledCounter == 1) {
            LED_On(LED_USB_DEVICE_STATE);
        } else if (ledCounter == 5000) {
            LED_Off(LED_USB_DEVICE_STATE);
        } else if (ledCounter == 10000) {
            ledCounter = 0;
        }

        uint16_t X_axis = ADC_Read10bit(PDL0);
        uint16_t Y_axis = ADC_Read10bit(PDL1);

        if (X_axis >= x_axis_max) {
            x_axis_max = X_axis;
        }
        if (X_axis <= x_axis_min) {
            x_axis_min = X_axis;
        }
        if (Y_axis >= y_axis_max) {
            y_axis_max = Y_axis;
        }
        if (Y_axis <= y_axis_min) {
            y_axis_min = Y_axis;
        }

        if (BUTTON_IsPressed(GAME_SWITCH_0) == true || BUTTON_IsPressed(GAME_SWITCH_1) == true) {

            while (BUTTON_IsPressed(GAME_SWITCH_0) == true || BUTTON_IsPressed(GAME_SWITCH_1) == true) {
                // wait
            }

            LED_Off(LED_USB_DEVICE_STATE);

            // Very arbitrary wait time so we can make sure the center value is stable
            uint16_t waitCount = 0;
            for (uint8_t i = 0; i < 40; i++) {
                while (waitCount < 65000) {
                    waitCount++;
                }
            }

            X_axis = ADC_Read10bit(PDL0);
            Y_axis = ADC_Read10bit(PDL1);

            x_axis_center = X_axis;
            y_axis_center = Y_axis;

            EepromWriteTwoBytes(X_AXIS_MIN_EEPROM_ADDRESS, x_axis_min);
            EepromWriteTwoBytes(X_AXIS_MID_EEPROM_ADDRESS, x_axis_center);
            EepromWriteTwoBytes(X_AXIS_MAX_EEPROM_ADDRESS, x_axis_max);

            EepromWriteTwoBytes(Y_AXIS_MIN_EEPROM_ADDRESS, y_axis_min);
            EepromWriteTwoBytes(Y_AXIS_MID_EEPROM_ADDRESS, y_axis_center);
            EepromWriteTwoBytes(Y_AXIS_MAX_EEPROM_ADDRESS, y_axis_max);

            calibrated = true;
        }

        ledCounter++;
    }
}

/**
 * Verifies the specified value is within the valid range
 * @param value the value to test
 * @return true if value is within the range, otherwise false
 */
bool isValidCalibrationValue(uint16_t value) {
    if (value <= JOYSTICK_10_BIT_MAX && value >= JOYSTICK_10_BIT_MIN) {
        return true;
    } else {
        return false;
    }
}

void JoystickUseSavedCalibration(uint16_t x_min, uint16_t x_mid, uint16_t x_max, uint16_t y_min, uint16_t y_mid, uint16_t y_max) {

    if (isValidCalibrationValue(x_min)) {
        x_axis_min = x_min;
    } else {
        x_axis_min = JOYSTICK_10_BIT_MIN;
    }

    if (isValidCalibrationValue(x_mid)) {
        x_axis_center = x_mid;
    } else {
        x_axis_center = JOYSTICK_10_BIT_MID;
    }

    if (isValidCalibrationValue(x_max)) {
        x_axis_max = x_max;
    } else {
        x_axis_max = JOYSTICK_10_BIT_MAX;
    }

    if (isValidCalibrationValue(y_min)) {
        y_axis_min = y_min;
    } else {
        y_axis_min = JOYSTICK_10_BIT_MIN;
    }

    if (isValidCalibrationValue(y_mid)) {
        y_axis_center = y_mid;
    } else {
        y_axis_center = JOYSTICK_10_BIT_MID;
    }

    if (isValidCalibrationValue(y_max)) {
        y_axis_max = y_max;
    } else {
        y_axis_max = JOYSTICK_10_BIT_MAX;
    }

    calibrated = true;
}

/**
 * Converts the ADC value to be within the calibration range.
 * @param ADCValue the 10 bit value read from the ADC
 * @param inputMin the min calibration value for this axis
 * @param inputMid the midpoint calibration value for this axis
 * @param inputMax the max calibration value for this axis
 * @return the final joystick value mapped to the calibration range
 */
uint8_t convertValue(uint16_t ADCValue, uint16_t inputMin, uint16_t inputMid, uint16_t inputMax) {

    // Prevent output from going beyond the min and max values
    if (ADCValue > inputMax) {
        ADCValue = inputMax;
    } else if (ADCValue < inputMin) {
        ADCValue = inputMin;
    }

    uint16_t input_end = inputMax;
    uint16_t input_start = inputMid;
    uint16_t output_end = JOYSTICK_8_BIT_MAX_OUTPUT;
    uint16_t output_start = JOYSTICK_8_BIT_MID_OUTPUT;
    if (ADCValue < inputMid) {
        input_end = inputMid - 1;
        input_start = inputMin;
        output_end = JOYSTICK_8_BIT_MID_OUTPUT - 1;
        output_start = JOYSTICK_8_BIT_MIN_OUTPUT;
    }
    double x_slope = 1.0 * (output_end - output_start) / (input_end - input_start);
    uint16_t sixteenBitOutput = output_start + x_slope * (ADCValue - input_start);

    uint8_t finalOutput = JOYSTICK_8_BIT_MAX_OUTPUT - (uint8_t) sixteenBitOutput;

    return finalOutput;
}

void JoystickTasks(void) {

    if (USBGetDeviceState() < CONFIGURED_STATE) {
        return;
    }

    if (USBIsDeviceSuspended() == true) {
        return;
    }

    //If the last transmission is complete
    if (!HIDTxHandleBusy(lastTransmission)) {

        if (joystick_counter > 5) {

            if (BUTTON_IsPressed(GAME_SWITCH_0) == true) {
                joystick_input.members.buttons.x = 1;
            } else {
                joystick_input.members.buttons.x = 0;
            }

            if (BUTTON_IsPressed(GAME_SWITCH_1) == true) {
                joystick_input.members.buttons.y = 1;
            } else {
                joystick_input.members.buttons.y = 0;
            }

            uint16_t X_axis = ADC_Read10bit(PDL0);
            uint16_t Y_axis = ADC_Read10bit(PDL1);

            uint8_t x_output = convertValue(X_axis, x_axis_min, x_axis_center, x_axis_max);
            uint8_t y_output = convertValue(Y_axis, y_axis_min, y_axis_center, y_axis_max);

            joystick_input.members.analog_stick.X = x_output;
            joystick_input.members.analog_stick.Y = y_output;

            joystick_counter = 0;
        } else {
            joystick_counter++;
        }

        //Send the packet over USB to the host.
        lastTransmission = HIDTxPacket(JOYSTICK_EP, (uint8_t*) & joystick_input, sizeof (joystick_input));
    }

}

#endif
