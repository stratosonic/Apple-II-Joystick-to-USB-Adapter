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
 * Joystick functions
 * www.flitey.com
 */

#ifndef USBJOYSTICK_H
#define	USBJOYSTICK_H
/**
 * Initializes the joystick endpoint
 */
void JoystickInitialize();

/**
 * Runs the calibration for the joystick potentiometers
 */
void JoystickRunCalibration();

/**
 * Sets the calibration values to the specified values
 * @param x_min the min value for the X axis
 * @param x_mid the mid value for the X axis
 * @param x_max the max value for the X axis
 * @param y_min the min value for the Y axis
 * @param y_mid the mid value for the Y axis
 * @param y_max the max value for the Y axis
 */
void JoystickUseSavedCalibration(uint16_t x_min, uint16_t x_mid, uint16_t x_max, uint16_t y_min, uint16_t y_mid, uint16_t y_max);

/**
 * Perform the joystick tasks which includes getting the values from the joystick
 * and then sending them to the computer.
 */
void JoystickTasks();

#endif	/* USBJOYSTICK_H */