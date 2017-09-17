/**
 * Mapping of IO
 */

#include "system.h"

#define GAME_SWITCH_0               BUTTON_S1
#define GAME_SWITCH_1               BUTTON_S2

#define PDL0                        ADC_CHANNEL_8
#define PDL1                        ADC_CHANNEL_9

#define LED_USB_DEVICE_STATE        LED_D1

#define X_AXIS_MIN_EEPROM_ADDRESS   0x00
#define X_AXIS_MID_EEPROM_ADDRESS   0x02
#define X_AXIS_MAX_EEPROM_ADDRESS   0x04

#define Y_AXIS_MIN_EEPROM_ADDRESS   0x06
#define Y_AXIS_MID_EEPROM_ADDRESS   0x08
#define Y_AXIS_MAX_EEPROM_ADDRESS   0x0A

#define JOYSTICK_10_BIT_MAX         1023
#define JOYSTICK_10_BIT_MID         511
#define JOYSTICK_10_BIT_MIN         0

#define JOYSTICK_8_BIT_MAX_OUTPUT   255
#define JOYSTICK_8_BIT_MID_OUTPUT   127
#define JOYSTICK_8_BIT_MIN_OUTPUT   0