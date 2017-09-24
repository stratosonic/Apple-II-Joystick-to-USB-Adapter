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
 * Mapping of IO
 * www.flitey.com
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